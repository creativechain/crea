Exchange Quickstart
-------------------

System Requirements: A dedicated server or virtual machine with a minimum of 8GB of RAM, and at least 30GB of fast local SSD storage.

With the right equipment and technical configuration a reindex should take **no longer than 2 hours**.  If recommendations are not followed precisely, the reindex can drag on for days or even weeks with significant slowdowns towards the end.

We recommend using docker to both build and run CREA for exchanges. Docker is the world's leading containerization platform and using it guarantees that your build and run environment is identical to what our developers use. You can still build from source and you can keep both blockchain data and wallet data outside of the docker container. The instructions below will show you how to do this in just a few easy steps.

### Install Docker

 - Install Docker on [Debian](https://docs.docker.com/install/linux/docker-ce/debian/)
 - Install Docker on [Ubuntu](https://docs.docker.com/install/linux/docker-ce/ubuntu/)
 - Install Docker on [Fedora](https://docs.docker.com/install/linux/docker-ce/fedora/)
 - Install Docker on [CentOS](https://docs.docker.com/install/linux/docker-ce/centos/)
 
### Install Git

 - On RPM-based Linux distribution
 ```bash
sudo dnf install git-all
``` 

 - On Debian-based Linux distribution
 ```bash
sudo apt install git-all
```

### Clone the crea repository

Pull in the crea repo from the official source on github and then change into the directory that's created for it.
```
git clone https://github.com/creativechain/crea
cd crea
```

### Build the image from source with docker

Docker isn't just for downloading already built images, it can be used to build from source the same way you would otherwise build. By doing this you ensure that your build environment is identical to what we use to develop the software. Use the below command to start the build:

```
docker build .
```

Don't forget the `.` at the end of the line which indicates the build target is in the current directory.

This will build everything including running our full suite of tests during the build process. It will anywhere from thirty minutes to a couple hours depending on how fast your equipment is.

When the build completes you will see a message indicating that it is 'successfully built'.

### Using our official Docker images without building from source

If you'd like to use our already pre-built official binary images, it's as simple as downloading it from the Dockerhub registry with only one command:

```
docker pull creary/crea
```

### Running a binary build without a Docker container

If you build with Docker but do not want to run cread from within a docker container, you can stop here with this step and instead extract the binary from the container with the commands below. If you are going to run cread with docker (recommended method), skip this step altogether. We're simply providing an option for everyone's use-case. Our binaries are built mostly static, only dynamically linking to linux kernel libraries. We have tested and confirmed binaries built in Docker work on Ubuntu and Fedora and will likely work on many other Linux distrubutions. Building the image yourself or pulling one of our pre-built images both work.

To extract the binary you need to start a container and then copy the file from it.

```
docker run -d --name cread-exchange creary/crea
docker cp cread-exchange:/usr/local/cread-default/bin/cread /local/path/to/cread
docker cp cread-exchange:/usr/local/cread-default/bin/cli_wallet /local/path/to/cli_wallet
docker stop cread-exchange
```

For your convenience, we have provided a provided an [example_config](example_config.ini) that we expect should be sufficient to run your exchange node. Be sure to rename it to simply `config.ini`.
Add [nodes](seednodes.txt) to your configuration file to perform full synchronization.

### Create directories to store blockchain and wallet data outside of Docker

For re-usability, you can create directories to store blockchain and wallet data and easily link them inside your docker container.

```
mkdir blockchain
mkdir creawallet
```

### Run the container

The below command will start a daemonized instance opening ports for p2p and RPC  while linking the directories we created for blockchain and wallet data inside the container. The `-v` flags are how you map directories outside of the container to the inside, you list the path to the directories you created earlier before the `:` for each `-v` flag. The restart policy ensures that the container will automatically restart even if your system is restarted.

```
docker run -d --name cread-exchange --env -p 1776:1776 -p 1996:1996 -v /path/to/creawallet:/var/creawallet -v /path/to/blockchain:/var/lib/cread/blockchain --restart always creary/crea
```

You can see that the container is running with the `docker ps` command.

To follow along with the logs, use `docker logs -f`.

Initial syncing will take between 5 minutes and 1 hour depending on your equipment, faster storage devices will take less time and be more efficient. Subsequent restarts will not take as long.

### Receive notifications

To receive notifications about an account it is necessary to have installed our developer and exchange package for Nodejs. You can see the documentation [here](https://github.com/creativechain/creary-tools/wiki).

### Running the cli_wallet

The command below will run the cli_wallet from inside the running container while mapping the `wallet.json` to the directory you created for it on the host.

```
docker exec -it cread-exchange /usr/local/cread-default/bin/cli_wallet -w /var/creawallet/wallet.json
```
