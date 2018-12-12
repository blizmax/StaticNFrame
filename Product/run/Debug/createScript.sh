cp NFPluginLoader NFMaster
chmod a+x NFMaster
cp runAll.sh runMaster.sh
chmod a+x runMaster.sh
sed -i "s/servername=NFPluginLoader/servername=NFMaster/g" ./runMaster.sh
sed -i "s/servertype=AllServer/servertype=MasterServer/g" ./runMaster.sh
sed -i "s/serverId=6/serverId=1/g" ./runMaster.sh

cp NFPluginLoader NFLogin
chmod a+x NFLogin
cp runAll.sh runLogin.sh
chmod a+x runLogin.sh
sed -i "s/servername=NFPluginLoader/servername=NFLogin/g" ./runLogin.sh
sed -i "s/servertype=AllServer/servertype=LoginServer/g" ./runLogin.sh
sed -i "s/serverId=6/serverId=4/g" ./runLogin.sh

cp NFPluginLoader NFWorld
chmod a+x NFWorld
cp runAll.sh runWorld.sh
chmod a+x runWorld.sh
sed -i "s/servername=NFPluginLoader/servername=NFWorld/g" ./runWorld.sh
sed -i "s/servertype=AllServer/servertype=WorldServer/g" ./runWorld.sh
sed -i "s/serverId=6/serverId=2/g" ./runWorld.sh

cp NFPluginLoader NFGame
chmod a+x NFGame
cp runAll.sh runGame.sh
chmod a+x runGame.sh
sed -i "s/servername=NFPluginLoader/servername=NFGame/g" ./runGame.sh
sed -i "s/servertype=AllServer/servertype=GameServer/g" ./runGame.sh
sed -i "s/serverId=6/serverId=10/g" ./runGame.sh

cp NFPluginLoader NFProxy
chmod a+x NFProxy
cp runAll.sh runProxy.sh
chmod a+x runProxy.sh
sed -i "s/servername=NFPluginLoader/servername=NFProxy/g" ./runProxy.sh
sed -i "s/servertype=AllServer/servertype=ProxyServer/g" ./runProxy.sh
sed -i "s/serverId=6/serverId=11/g" ./runProxy.sh
