
--该类是存放麻将常用的一些固定定义和列表。
--避免在每个函数里面重复构建。


MajiangDefine = {}

MajiangDefine.mark_shisanyao = {11,19,21,29,31,39,41,42,43,44,51,52,53}  --十三幺的判断列表，统一放到这里
MajiangDefine.mark_jiulanbaodeng = {1,1,1,2,3,4,5,6,7,8,9,9,9}
MajiangDefine.mark_alllist = {11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37,38,39,41,42,43,44,51,52,53}

MajiangDefine.mark_jiangHuArrNum = {2,5,8,11,14} --有将情况下需要胡的类型
MajiangDefine.mark_huArrNum = {0,3,9,12}      --无将情况下需要的数量

--1番
MajiangDefine.kwx_pihu = 101     --屁胡
--2番
MajiangDefine.kwx_pengpenghu = 102    --碰碰胡
MajiangDefine.kwx_mingsiguiquan = 103 --全频道的明四归一
MajiangDefine.kwx_mingsiguiban = 104  --半频道的明四归一
MajiangDefine.kwx_gangshanghua = 105  --杠上花
MajiangDefine.kwx_gangshangpao = 106  --杠上炮
MajiangDefine.kwx_qiangganghu = 107   --抢杠胡
MajiangDefine.kwx_mingpai = 108       --明牌
MajiangDefine.kwx_haidilao = 109   --海底捞
MajiangDefine.kwx_haidipao = 110      --海底炮
MajiangDefine.kwx_baohu = 111        --包胡：放炮给亮倒玩家
MajiangDefine.kwx_kawuxing = 112   --卡五星





--4番
MajiangDefine.kwx_qingyise = 113    --清一色
MajiangDefine.kwx_qidui = 114     	 --七对
MajiangDefine.kwx_ansiguiquan = 115   --暗四归：全频道
MajiangDefine.kwx_ansiguiban = 116    --暗四归: 半频道
MajiangDefine.kwx_shouzhuayi = 117   --手抓一
MajiangDefine.kwx_xiaosanyuan = 118  --小三元
--8番
MajiangDefine.kwx_hhqd = 119   --豪华七对
MajiangDefine.kwx_dasanyuan = 120  --大三元
--32番
MajiangDefine.kwx_chhqd = 121  --超豪华七对
--128番
MajiangDefine.kwx_cchhqd = 122  --超超豪华七对

------
MajiangDefine.kwx_zimo = 123 --这个仅仅是为了显示用的，如果都存在上面的类型，而玩家又是自摸，那就显示自摸


