Exchange Quickstart
-------------------

System Requirements: A dedicated server or virtual machine with a minimum of 16GB of RAM, and at least 50GB of fast local SSD storage. CREA is one of the most active blockchains in the world and handles an incredibly large amount of transactions per second, as such, it requires fast storage to run efficiently.

With the right equipment and technical configuration a reindex should take **no longer than 24 hours**.  If recommendations are not followed precisely, the reindex can drag on for days or even weeks with significant slowdowns towards the end.

Physically attached SSD will ensure an optimal reindex time.  SSD over a NAS or some kind of network storage backed by SSD will often have much higher latency. As an example, AWS EBS is not performant enough. A good recommended instance in AWS is the i3.2xlarge, it comes with a physically attached nVME drive (it must be formatted and mounted on instance launch).

You can save a lot of time by replaying from a `block_log`. Creait hosts a public `block_log` that is regularly updated. Using the docker method below, we have made it easy to download a `block_log` at launch and replay from it by passing in the `USE_PUBLIC_BLOCKLOG=1` environment variable. To do this, make sure your data directory is empty and does not contain a block_log. If you are not using docker, you can download a `block_log` from [here](https://s3.amazonaws.com/creait-dev-blockchainstate/block_log-latest), put it in your crea data directory, and use the `--replay-blockchain` command line option. Be sure to remove the option if you have to stop/restart cread after already being synced.

We recommend using docker to both build and run CREA for exchanges. Docker is the world's leading containerization platform and using it guarantees that your build and run environment is identical to what our developers use. You can still build from source and you can keep both blockchain data and wallet data outside of the docker container. The instructions below will show you how to do this in just a few easy steps.

### Install docker and git (if not already installed)

On Ubuntu 16.04+:
```
apt-get update && apt-get install git docker.io -y
```

On other distributions you can install docker with the native package manager or with the script from get.docker.com:
```
curl -fsSL get.docker.com -o get-docker.sh
sh get-docker.sh
```

### Clone the crea repo

Pull in the crea repo from the official source on github and then change into the directory that's created for it.
```
git clone https://github.com/creativechain/crea
cd crea
```

### Build the image from source with docker

Docker isn't just for downloading already built images, it can be used to build from source the same way you would otherwise build. By doing this you ensure that your build environment is identical to what we use to develop the software. Use the below command to start the build:

```
docker build -t=creativechain/crea .
```

Don't forget the `.` at the end of the line which indicates the build target is in the current directory.

This will build everything including running our full suite of tests during the build process. It will anywhere from thirty minutes to a couple hours depending on how fast your equipment is.

When the build completes you will see a message indicating that it is 'successfully built'.

### Using our official Docker images without building from source

If you'd like to use our already pre-built official binary images, it's as simple as downloading it from the Dockerhub registry with only one command:

```
docker pull creativechain/crea
```

### Running a binary build without a Docker container

If you build with Docker but do not want to run cread from within a docker container, you can stop here with this step and instead extract the binary from the container with the commands below. If you are going to run cread with docker (recommended method), skip this step altogether. We're simply providing an option for everyone's use-case. Our binaries are built mostly static, only dynamically linking to linux kernel libraries. We have tested and confirmed binaries built in Docker work on Ubuntu and Fedora and will likely work on many other Linux distrubutions. Building the image yourself or pulling one of our pre-built images both work.

To extract the binary you need to start a container and then copy the file from it.

```
docker run -d --name cread-exchange creativechain/crea
docker cp cread-exchange:/usr/local/cread-default/bin/cread /local/path/to/cread
docker cp cread-exchange:/usr/local/cread-default/bin/cli_wallet /local/path/to/cli_wallet
docker stop cread-exchange
```

For your convenience, we have provided a provided an [example_config](example_config.ini) that we expect should be sufficient to run your exchange node. Be sure to rename it to simply `config.ini`.

### Create directories to store blockchain and wallet data outside of Docker

For re-usability, you can create directories to store blockchain and wallet data and easily link them inside your docker container.

```
mkdir blockchain
mkdir creawallet
```

### Run the container

The below command will start a daemonized instance opening ports for p2p and RPC  while linking the directories we created for blockchain and wallet data inside the container. Fill in `TRACK_ACCOUNT` with the name of your exchange account that you want to follow. The `-v` flags are how you map directories outside of the container to the inside, you list the path to the directories you created earlier before the `:` for each `-v` flag. The restart policy ensures that the container will automatically restart even if your system is restarted.

```
docker run -d --name cread-exchange --env TRACK_ACCOUNT=nameofaccount --env USE_PUBLIC_BLOCKLOG=1 -p 1776:1776 -p 1886:1886 -v /path/to/creawallet:/var/creawallet -v /path/to/blockchain:/var/lib/cread/blockchain --restart always creait/crea
```

You can see that the container is running with the `docker ps` command.

To follow along with the logs, use `docker logs -f`.

Initial syncing will take between 6 and 48 hours depending on your equipment, faster storage devices will take less time and be more efficient. Subsequent restarts will not take as long.

### Running the cli_wallet

The command below will run the cli_wallet from inside the running container while mapping the `wallet.json` to the directory you created for it on the host.

```
docker exec -it cread-exchange /usr/local/cread-default/bin/cli_wallet -w /var/creawallet/wallet.json
```
