g_PdkDefine = {}

--��������
g_PdkDefine.chat_txt = 1
g_PdkDefine.chat_img = 2
g_PdkDefine.chat_voice = 3

--����֧����ʽ
g_PdkDefine.payment_owner = 1 		--����֧��
g_PdkDefine.payment_A = 2     		--AA֧��

--֧������
g_PdkDefine.pay_money = 1001 		--��ʯ֧��
g_PdkDefine.pay_jetton = 1002		--���֧��
g_PdkDefine.pay_gold =  1003		--��֧��

--֧������
g_PdkDefine.diamond_pay = 0 		--��ʯ֧��
g_PdkDefine.jade_pay =  1		--��֧��

 --������������
g_PdkjType = {}
g_PdkjType.general = 201 			--�����淨

g_PdkDefine.max_user = 3			--�������
g_PdkDefine.table_elementary = 101	--������
g_PdkDefine.table_middle = 102		--�м���
g_PdkDefine.table_jadvanced = 103	--�߼���
g_PdkDefine.table_srf = 201 		--˽�˳�

--������״̬
g_PdkStatus = {}
g_PdkStatus.state_ready = 1			--�ȴ���Ϸ��ʼ
g_PdkStatus.status_start  = 2    	--��Ϸ��ʼ��״̬
g_PdkStatus.state_play  = 3			--����ж���״̬
g_PdkStatus.state_count  = 4		--�����״̬    
g_PdkStatus.state_dissolve = 5  	--�����ɢ��״̬
g_PdkStatus.state_delete  = 6  		--�ȴ�ɾ��״̬��״̬
g_PdkStatus.state_next  = 7  		--�ȴ���һ����Ϸ��ʼ��״̬


--������״̬���е�ʱ��
g_PdkTime = {}
g_PdkTime.ready_time = 1800 		--1800��Ϸ��û��ʼ�Զ���ɢ����
g_PdkTime.start_time = 1			--��Ϸ��ʼ����������ʱ��
g_PdkTime.play_time =  12			--ÿλ���ÿ�صĲ�����ʱ��
g_PdkTime.count_time = 1			--���㼰��������ʱ��
g_PdkTime.dissolve_time = 300 		--�����ɢ��ʱ����Ľ�ɢʱ��
g_PdkTime.next_time = 60 			--�ȴ���һ����Ϸ��ʼʱ��

--���״̬
g_PdkPlayerState = {}
g_PdkPlayerState.status_notReady = 100
g_PdkPlayerState.status_ready = 101	 --׼��		
g_PdkPlayerState.status_wait = 102   --�ȴ��ж�
g_PdkPlayerState.status_play = 103	 --�����ж�


--��ҳ�������
g_PdkPlay_poker = {}
g_PdkPlay_poker.doNot = 1				--��
g_PdkPlay_poker.play = 2				--����


--����ж�����
g_PdkAction = {}
g_PdkAction.type_play  = 3 			--����


--����
g_PdkCard_type = {}
g_PdkCard_type.invalid = 0 				--��������
g_PdkCard_type.danzhang = 1     		--����
g_PdkCard_type.duizi = 2     			--����
g_PdkCard_type.shunzi = 3    			--˳��
g_PdkCard_type.sanshunzi = 4			--��ͬ��˳��
g_PdkCard_type.liandui = 5    			--����
g_PdkCard_type.feiji = 6    			--�ɻ�
g_PdkCard_type.sanzhang = 7				--��ͬ��(ֻ���������һ����)
g_PdkCard_type.sandaiyi = 8    			--����һ(ֻ���������һ����)
g_PdkCard_type.sandaier = 9    			--������
g_PdkCard_type.sidaisan = 10				--�Ĵ���(ֻ�����ڹ�ѡ�Ĵ���)
g_PdkCard_type.stiffBomb = 11    		--Ӳը��()
g_PdkCard_type.threeABomb = 12    		--3Aը��(ֻ�����ڹ�ѡ3Aը���淨)
g_PdkCard_type.threeAoneBomb = 13		--3A��һը��(ֻ�����ڹ�ѡ3A��һ��ը���淨)


--�ƶ���												
g_PdkPokerList = {101,102,103,104,105,106,107,108,109,110,111,112,113,
				  201,202,203,204,205,206,207,208,209,210,211,212,213,
				  301,302,303,304,305,306,307,308,309,310,311,312,313,
				  401,402,403,404,405,406,407,408,409,410,411,412,413,
				  }   --��������С����			1���� 2÷�� 3���� 4����


--�й�
g_PdkDefine.autoplayer_No = 0			--ȡ���й�
g_PdkDefine.autoplayer_Yes = 1			--�й�

--����
g_PdkDefine.spring_No = 0				--���Ǵ���
g_PdkDefine.spring_Yes = 1				--����


g_PdkDefine.spc_win = 101
g_PdkDefine.spc_lose = 102
