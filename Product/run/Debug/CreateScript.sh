
do_work()
{
	cp NFPluginLoader $1 -rf
	chmod a+x $1
	cp AllServer.sh run$1$2.sh -rf
	chmod a+x run$1$2.sh 
	cp AllServer_log.sh $1_log.sh -rf
	chmod a+x $1_log.sh
	sed -i "s/servername=NFPluginLoader/servername=$1/g" ./run$1$2.sh
	sed -i "s/servertype=AllServer/servertype=$1/g" ./run$1$2.sh
	sed -i "s/serverId=6/serverId=$2/g" ./run$1$2.sh

	sed -i "s/AllServer/$1/g" ./$1_log.sh
}

do_work MasterServer 1
do_work WorldServer 2
do_work LoginServer 4
do_work GameServer 10
do_work ProxyServer 11

