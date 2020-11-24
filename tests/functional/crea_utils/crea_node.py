#!/usr/bin/python3

import json
import logging
import sys
import os
import subprocess
import datetime

from .common import DEFAULT_LOG_FORMAT, DEFAULT_LOG_LEVEL

from threading import Lock

MODULE_NAME = "hive-utils.crea-node"

logger = logging.getLogger(MODULE_NAME)
logger.setLevel(DEFAULT_LOG_LEVEL)

ch = logging.StreamHandler(sys.stdout)
ch.setLevel(DEFAULT_LOG_LEVEL)
ch.setFormatter(logging.Formatter(DEFAULT_LOG_FORMAT))

logger.addHandler(ch)

from typing import NamedTuple

class HiveNode(object):
  cread_binary = None
  cread_process = None
  cread_lock = Lock()
  cread_data_dir = None
  cread_args = list()

  def __init__(self, binary_path : str, working_dir : str, binary_args : list):
    logger.info("New hive node")
    if not os.path.exists(binary_path):
      raise ValueError("Path to cread binary is not valid.")
    if not os.path.isfile(binary_path):
      raise ValueError("Path to cread binary must point to file")
    self.cread_binary = binary_path

    if not os.path.exists(working_dir):
      raise ValueError("Path to data directory is not valid")
    if not os.path.isdir(working_dir):
      raise ValueError("Data directory is not valid directory")
    self.cread_data_dir = working_dir

    if binary_args:
      self.cread_args.extend(binary_args)

  def __enter__(self):
    self.cread_lock.acquire()

    from subprocess import Popen, PIPE
    from time import sleep

    cread_command = [
      self.cread_binary,
      "--data-dir={}".format(self.cread_data_dir)
    ]
    cread_command.extend(self.cread_args)

    self.cread_process = Popen(cread_command, stdout=PIPE, stderr=None)
    self.cread_process.poll()
    sleep(5)

    if self.cread_process.returncode:
      raise Exception("Error during starting node")

  def get_output(self):
    out, err = self.cread_process.communicate()
    return out

  def __exit__(self, exc, value, tb):
    logger.info("Closing node")
    from signal import SIGINT, SIGTERM
    from time import sleep

    if self.cread_process is not None:
      self.cread_process.poll()
      if self.cread_process.returncode != 0:
        self.cread_process.send_signal(SIGINT)
        sleep(7)
        self.cread_process.poll()
        if self.cread_process.returncode != 0:
          self.cread_process.send_signal(SIGTERM)
          sleep(7)
          self.cread_process.poll()
          if self.cread_process.returncode != 0:
            raise Exception("Error during stopping node. Manual intervention required.")
    self.cread_process = None
    self.cread_lock.release()

class HiveNodeInScreen(object):
  def __init__(self, crea_executable, working_dir, config_src_path, run_using_existing_data = False, node_is_steem = False):
    self.crea_executable = crea_executable
    self.working_dir = working_dir
    self.config_src_path = config_src_path
    self.node_is_steem = node_is_steem

    # usefull when we want to do a replay
    if not run_using_existing_data:
      from shutil import rmtree, copy
      # remove old data from node
      if os.path.exists(self.working_dir):
        rmtree(self.working_dir)
      os.makedirs(self.working_dir+"/blockchain")
      # copy config file to working dir
      copy(self.config_src_path, self.working_dir + "/config.ini")

    self.crea_config = self.parse_node_config_file(self.working_dir + "/config.ini")
    self.ip_address, self.port = self.crea_config["webserver-http-endpoint"][0].split(":")
    self.ip_address = "http://{}".format(self.ip_address)
    self.node_running = False

  def get_from_config(self, key):
    return self.crea_config.get(key, None)

  def get_node_url(self):
    return "{}:{}/".format(self.ip_address, self.port)

  def is_running(self):
    return self.node_running

  def parse_node_config_file(self, config_file_name):
    ret = dict()
    lines = None
    with open(config_file_name, "r") as f:
      lines = f.readlines()

    for line in lines:
      proc_line = line.strip()
      if proc_line:
        if proc_line.startswith("#"):
            continue
        k, v = proc_line.split("=", 1)
        k = k.strip()
        v = v.strip()
        if k in ret:
          ret[k].append(v)
        else:
          ret[k] = [v]
    return ret

  def run_crea_node(self, additional_params = [], wait_for_blocks = True):
    from .common import detect_process_by_name, save_screen_cfg, save_pid_file, wait_n_blocks, wait_for_string_in_file, kill_process

    if detect_process_by_name("cread" if not self.node_is_steem else "steemd", self.crea_executable, self.port):
      msg = "{0} process is running on {1}:{2}. Please terminate that process and try again.".format("hive", self.ip_address, self.port)
      raise ProcessLookupError(msg)

    logger.info("*** START NODE at {0}:{1} in {2}".format(self.ip_address, self.port, self.working_dir))

    parameters = [
      self.crea_executable,
      "-d",
      self.working_dir,
      "--advanced-benchmark",
      "--sps-remove-threshold",
      "-1"
    ]

    parameters = parameters + additional_params
    
    self.pid_file_name = "{0}/run_crea-{1}.pid".format(self.working_dir, self.port)
    current_time_str = datetime.datetime.now().strftime("%Y-%m-%d")
    log_file_name = "{0}/{1}-{2}-{3}.log".format(self.working_dir, "hive", self.port, current_time_str)
    screen_cfg_name = "{0}/crea_screen-{1}.cfg".format(self.working_dir, self.port)

    save_screen_cfg(screen_cfg_name, log_file_name)
    screen_params = [
      "screen",
      "-m",
      "-d",
      "-L",
      "-c",
      screen_cfg_name,
      "-S",
      "{0}-{1}-{2}".format("hive", self.port, current_time_str)
    ]

    parameters = screen_params + parameters
    logger.info("Running cread with command: {0}".format(" ".join(parameters)))
      
    try:
      subprocess.Popen(parameters)
      save_pid_file(self.pid_file_name, "hive", self.port, current_time_str)
      # we will allow for screen to setup and die maybe?
      from time import sleep
      sleep(5)
      # now it should be dead

      if not detect_process_by_name("cread" if not self.node_is_steem else "steemd", self.crea_executable, self.port):
        msg = "{0} process is not running on {1}:{2}. Please check logs.".format("hive", self.ip_address, self.port)
        raise ProcessLookupError(msg)

      if "--replay-blockchain" in parameters:
        wait_for_string_in_file(log_file_name, "start listening for ws requests", None)
      else:
        if wait_for_blocks:
          wait_n_blocks("{}:{}".format(self.ip_address, self.port), 5)
        else:
          wait_for_string_in_file(log_file_name, "start listening for ws requests", 20.)
      self.node_running = True
      logger.info("Node at {0}:{1} in {2} is up and running...".format(self.ip_address, self.port, self.working_dir))
    except Exception as ex:
      logger.error("Exception during cread run: {0}".format(ex))
      kill_process(self.pid_file_name, "hive" if not self.node_is_steem else "steem", self.ip_address, self.port)
      self.node_running = False


  def stop_crea_node(self):
    from .common import kill_process
    logger.info("Stopping node at {0}:{1}".format(self.ip_address, self.port))
    kill_process(self.pid_file_name, "hive" if not self.node_is_steem else "steem", self.ip_address, self.port)
    self.node_running = False

  def __enter__(self):
    self.run_crea_node()

  def __exit__(self, exc, value, tb):
    self.stop_crea_node()

if __name__ == "__main__":
  KEEP_GOING = True

  def sigint_handler(signum, frame):
    logger.info("Shutting down...")
    global KEEP_GOING
    from time import sleep
    KEEP_GOING = False
    sleep(3)
    sys.exit(0)

  def main():
    try:
      import signal
      signal.signal(signal.SIGINT, sigint_handler)

      plugins = ["chain","p2p","webserver","json_rpc","debug_node"]
      config = "# Simple config file\n" \
        + "shared-file-size = 1G\n" \
        + "enable-stale-production = true\n" \
        + "p2p-endpoint = 127.0.0.1:2001\n" \
        + "webserver-http-endpoint = 127.0.0.1:8095\n" \
        + "webserver-ws-endpoint = 127.0.0.1:8096\n" \
        + "plugin = witness debug_node {}\n".format(" ".join(plugins)) \
        + "plugin = database_api debug_node_api block_api\n" \
        + "witness = \"initminer\"\n" \
        + "private-key = 5JNHfZYKGaomSFvd4NUdQ9qMcEAC43kujbfjueTHpVapX1Kzq2n\n" \
        + "required-participation = 0"

      binary_path = "/home/dariusz-work/Builds/crea/programs/cread/cread"
      work_dir = "/home/dariusz-work/crea-data"

      print(config)

      with open(work_dir + "/config.ini", "w") as conf_file:
        conf_file.write(config)

      node = HiveNode(binary_path, work_dir, [])
      from time import sleep
      from .common import wait_n_blocks, debug_generate_blocks
      with node:
        print("Waiting 10 blocks")
        wait_n_blocks("http://127.0.0.1:8095", 10)
        print("Done...")
        print(debug_generate_blocks("http://127.0.0.1:8095", "5JHNbFNDg834SFj8CMArV6YW7td4zrPzXveqTfaShmYVuYNeK69", 100))
        while(KEEP_GOING):
          sleep(1)
    except Exception as ex:
      logger.exception("Exception: {}".format(ex))
      sys.exit(1)
  
  main()



