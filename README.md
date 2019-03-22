![alt text](https://i.imgur.com/P04jzp4.png)


## Introducing Crea (beta)

# Crea
#### Decentralized Creative Communities
Platform for the creation of hypermedia dapps based on the #blockchain that rewards creatives and curators of digital content. 


## DPOS-Decentralized
Thanks to the integration of the Delegated Proof of Stake (DPOS) consensus protocol, Crea’s blockchain is maintained in a decentralized way by community members from around the world.
DPOS uses the voting power of users to solve consensus problems efficiently, fairly and democratically.

## REWARDS-Smart Media Rewards
Proof of Creation is a type of algorithm that issues rewards in tokens for the creation and curation of content. Users decide with their “likes” as votes which publications should be rewarded by the system.
Any online platform can easily integrate this token distribution system and create decentralized applications (DAPPS). This distributed monetization model promotes the creation and collective work while distributing power among the members of the community.

## FAST-Speed and scalability
Thanks to Graphene technology, the Crea network can support the issuance of more than 1000 Transactions Per Second (TPS) and without network fees. This amount of TPS is necessary to sustain the massive volume of interactions in the new generation social networks.


## BENEFITS IN THE FIELD OF DIGITAL CREATION

### Proof of Existence
Certificate of creation registration based on the blockchain.

### Proof of Creation (POC)
An algorithm rewards authors and curators for their work.

### Reputation & Voting power
Those who get more tokens will have more influence in the community.

### Decentralized Exchange (DEX)
Crea tokens exchange markets without fees.

### Open Source
Our ecosystem is completely open source to help you meet your needs when launching your application projects.

### Control of inflation
Issuance of new tokens using a formula that controls the annual inflation rate.

### Eliminating Transaction Fees
An advantage of the CREA blockchain over centralized applications competitors.

### Censorship resistance
The distributed consensus protocol of Crea prevents the centralization and the censorship of content.


## Quickstart

Just want to get up and running quickly? We have pre-built docker images for your convenience. More details are in our 
[quickstart guide mainnet](doc/exchangequickstart.md). 
[quickstart guide test](https://github.com/creativechain/crea/blob/test/doc/exchangequickstart.md).

## Building

We strongly recommend using one of our pre-built Docker images or using Docker to build Crea. Both of these processes are described in the [quickstart guide](doc/exchangequickstart.md).
See [doc/building.md](doc/building.md) for detailed build instructions, including
compile-time options, and specific commands for Linux (Ubuntu LTS) or macOS.

## Quickstart

### Dockerized p2p Node
To run a p2p node (ca. 2GB of memory is required at the moment):

    docker run \
        -d -p 1776:1776 -p 1886:1886 --name cread-default \
        creary/crea

    docker logs -f cread-default  # follow along

### Dockerized Full Node

To run a node with *all* the data (e.g. for supporting a content website)
ca. 8GB of memory, and growing, is required:

    docker run \
        --env USE_WAY_TOO_MUCH_RAM=1 --env USE_FULL_WEB_NODE=1 \
        -d -p 1776:1776 -p 1886:1886 --name cread-full \
        creary/crea

    docker logs -f cread-full

## Environment variables

* `USE_WAY_TOO_MUCH_RAM` - if set to true, cread starts a 'full node'
* `USE_FULL_WEB_NODE` - if set to true, a default config file will be used that enables a full set of API's and associated plugins.
* `USE_NGINX_FRONTEND` - if set to true, this will enable an NGINX reverse proxy in front of cread that proxies WebSocket requests to cread. This will also enable a custom healthcheck at the path '/health' that lists how many seconds away from current blockchain time your node is. It will return a '200' if it's less than 60 seconds away from being synced.
* `USE_MULTICORE_READONLY` - if set to true, this will enable cread in multiple reader mode to take advantage of multiple cores (if available). Read requests are handled by the read-only nodes and write requests are forwarded back to the single 'writer' node automatically. NGINX load balances all requests to the reader nodes, 4 per available core. This setting is still considered experimental and may have trouble with some API calls until further development is completed.
* `HOME` - set this to the path where you want cread to store it's data files (block log, shared memory, config file, etc). By default `/var/lib/cread` is used and exists inside the docker container. If you want to use a different mount point (like a ramdisk, or a different drive) then you may want to set this variable to map the volume to your docker container.

We provide a basic cli wallet for interfacing with `cread`. The wallet is self-documented via command line help. The node you connect to via the cli wallet needs to be running the `account_by_key_api`, `condenser_api`, and needs to be configured to accept WebSocket connections via `webserver-ws-endpoint`.

# Configuration

## Config File

Run `cread` once to generate a data directory and config file. The default location is `witness_node_data_dir`. Kill `cread`. It won't do anything without seed nodes. If you want to modify the config to your liking, we have two example configs used in the docker images. ( [consensus node](contrib/config-for-docker.ini), [full node](contrib/fullnode.config.ini) ) All options will be present in the default config file and there may be more options needing to be changed from the docker configs (some of the options actually used in images are configured via command line).

## Seed Nodes

A list of some seed nodes to get you started can be found in
[doc/seednodes.txt](doc/seednodes.txt).

This same file is baked into the docker images and can be overridden by
setting `CREAD_SEED_NODES` in the container environment at `docker run`
time to a whitespace delimited list of seed nodes (with port).

## CLI Wallet

We provide a basic cli wallet for interfacing with `cread`. The wallet is self documented via command line help. The node you connect to via the cli wallet needs to be running the `account_by_key_api`, `condenser_api`, and needs to be configured to accept websocket connections via `webserver-ws-endpoint`.

## System Requirements

For a full web node, you need at least 25GB of disk space available. Creativecoind uses a memory mapped file which currently holds 56GB of data and by default is set to use up to 80GB. The block log of the blockchain itself is a little over 27GB. It's highly recommended to run cread on a fast disk such as an SSD or by placing the shared memory files in a ramdisk and using the `--shared-file-dir=/path` command line option to specify where. At least 16GB of memory is required for a full web node. Seed nodes (p2p mode) can run with as little as 4GB of memory with a 24 GB state file. Any CPU with decent single core performance should be sufficient.
On Linux use the following Virtual Memory configuration for the initial sync and subsequent replays. It is not needed for normal operation.

```
echo    75 | sudo tee /proc/sys/vm/dirty_background_ratio
echo  1000 | sudo tee /proc/sys/vm/dirty_expire_centisecs
echo    80 | sudo tee /proc/sys/vm/dirty_ratio
echo 30000 | sudo tee /proc/sys/vm/dirty_writeback_centisecs
```

## No Support & No Warranty

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

 ## Public Announcement & Discussion

Crea was announced on the
[Bitcointalk forum](https://bitcointalk.org/index.php?topic=1809920.0)

The members of the Creativechain foundation & creaproject.io. development team are currently the main contributors to the Crea Network software. They oversee the open source Create GitHub repository, and maintain many of the open source libraries that developers use.
To get any help regarding the development of CREA Network you can access Crea Devs Chat in Discord https://discord.gg/XZKGbZb .
Create Devs Channel is a public Discord chat community where members of the Create development community go to discuss Create development, and other related topics.
