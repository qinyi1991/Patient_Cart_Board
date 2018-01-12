/***********************����appȫ�ֱ���***************************************************************************/
#ifndef __MODUS_CONTROLTARGETCONF_H
#define __MODUS_CONTROLTARGETCONF_H

/*
*********************************************************************************************************
*                                               MODBUS DEFINE
*********************************************************************************************************
*/
//0-400 ���� U8��λ ӳ��Ĵ���0-200
//��ѭ��ʽ ModbusAdd(n) ӳ�䵽EEPRO
/*----------------------  register address ----------------------------------*/   
#define 		SRegisAdd1		1		//�趨ֵ
#define 		SRegisAdd2		2		//
#define 		SRegisAdd3		3		//
#define 		SRegisAdd4		4		//
#define 		SRegisAdd5		5		//
#define 		SRegisAdd6		6		//
#define 		SRegisAdd7		7		//
#define 		SRegisAdd8		8		//
#define 		SRegisAdd9		9		//
#define 		SRegisAdd10		10		//
#define 		SRegisAdd11		11		//
#define 		SRegisAdd12		12		//
#define 		SRegisAdd13		13		//
#define 		SRegisAdd14		14		//
#define 		SRegisAdd15		15		//
#define 		SRegisAdd16		16		//
#define 		SRegisAdd17		17		//
#define 		SRegisAdd18		18		//
 
//����Ĵ�������
//400-800 ӳ��Ĵ��� 200-400
#define 		NRegisAdd101		101		//101 CH1
#define 		NRegisAdd102		102		//101 CH2
#define 		NRegisAdd103		103		//101 CH3
#define 		NRegisAdd104		104		//101 CH4
#define 		NRegisAdd105		105		//101 CH5
#define 		NRegisAdd106		106		//101 CH6
#define 		NRegisAdd107		107		//101 CH7
#define 		NRegisAdd108		108		//101 CH8
#define 		NRegisAdd109		109		//101 CH9
#define 		NRegisAdd110		110		//101 CH10
#define 		NRegisAdd111		111		//101 CH11
#define 		NRegisAdd112		112		//101 CH12
#define 		NRegisAdd113		113		//101 CH13
#define 		NRegisAdd114		114		//101 CH14
#define 		NRegisAdd115		115		//101 CH15
#define 		NRegisAdd116		116		//101 CH16
#define 		NRegisAdd117		117		//101 CH17
#define 		NRegisAdd118		118		//101 CH18
#define 		NRegisAdd119		119		//101 CH19
#define 		NRegisAdd120		120		//101 CH20
#define 		NRegisAdd121		121		//101 CH21
#define 		NRegisAdd122		122		//101 CH22
#define 		NRegisAdd123		123		//101 CH23
#define 		NRegisAdd124		124		//101 CH24
#define 		NRegisAdd125		125		//101 CH25
#define 		NRegisAdd126		126		//101 CH26
#define 		NRegisAdd127		127		//101 CH27
#define 		NRegisAdd128		128		//101 CH28
#define 		NRegisAdd129		129		//101 CH29
#define 		NRegisAdd130		130		//101 CH30
#define 		NRegisAdd131		131		//101 CH31
#define 		NRegisAdd132		132		//101 CH32
#define 		NRegisAdd133		133		//101 CH33
#define 		NRegisAdd134		134		//101 CH34
#define 		NRegisAdd135		135		//101 CH35
#define 		NRegisAdd136		136		//101 CH36
#define 		NRegisAdd137		137		//101 CH37
#define 		NRegisAdd138		138		//101 CH38
#define 		NRegisAdd139		139		//101 CH39

#define 		NRegisAdd201		201		//101 CH40       
/*---------------------- coils  address ----------------------------------*/
//��ѭ��ʽ OperateAdd(SCoilAdd1,adr) ӳ�䵽EEPRO
//coil 0--400
//������Ȧ
#define 		SCoilAdd1		1		//
#define 		SCoilAdd2		2		//
#define 		SCoilAdd3		3		//
#define 		SCoilAdd4		4		//
#define 		SCoilAdd5		5		//
#define 		SCoilAdd6		6		//
#define 		SCoilAdd7		7		//
#define 		SCoilAdd8		8		//
#define 		SCoilAdd9		9		//
#define 		SCoilAdd10		10		//
#define 		SCoilAdd11		11		//
#define 		SCoilAdd12		12		//
#define 		SCoilAdd13		13		//
#define 		SCoilAdd14		14		//
#define 		SCoilAdd15		15		//
#define 		SCoilAdd16		16		//
#define 		SCoilAdd17		17		//
#define 		SCoilAdd18		18		//
#define 		SCoilAdd19		19		//
#define 		SCoilAdd20		20		//

//-----------������״̬��ַ----------------
//400--800 write in eepram 400--800 not save
//��̬��Ȧ
#define		NCoilAdd401		401		//1#��Ӧ�¼�
#define		NCoilAdd402		402		//2#��Ӧ�¼� 
#define		NCoilAdd403		403		//3#��Ӧ�¼�
#define		NCoilAdd404		404		//4#��Ӧ�¼�
#define		NCoilAdd405		405		//5#��Ӧ�¼�
#define		NCoilAdd406		406		//6#��Ӧ�¼�
#define		NCoilAdd407		407		//7#��Ӧ�¼�
#define		NCoilAdd408		408		//8#��Ӧ�¼�
#define		NCoilAdd409		409		//9#��Ӧ�¼�
#define		NCoilAdd410		410		//10#��Ӧ�¼�
#define		NCoilAdd411		411		//11#��Ӧ�¼�
#define		NCoilAdd412		412		//12#��Ӧ�¼�
#define		NCoilAdd413		413		//13#��Ӧ�¼�
#define		NCoilAdd414		414		//14#��Ӧ�¼�
#define		NCoilAdd415		415		//15#��Ӧ�¼�
#define		NCoilAdd416		416		//16#��Ӧ�¼�
#define		NCoilAdd417		417		//17#��Ӧ�¼�
//������¼
#define		NCoilAdd440		440		//1#�����¼�
#define		NCoilAdd441		441		//2#�����¼�
#define		NCoilAdd442		442		//3#�����¼�
#define		NCoilAdd443		443		//4#�����¼�
#define		NCoilAdd444		444		//5#�����¼�
#define		NCoilAdd445		445		//6#�����¼�
#define		NCoilAdd446		446		//7#�����¼�
#define		NCoilAdd447		447		//8#�����¼�
#define		NCoilAdd448		448		//9#�����¼�
#define		NCoilAdd449		449		//10#�����¼�
#define		NCoilAdd450		450		//11#�����¼�
#define		NCoilAdd451		451		//12#�����¼�
#define		NCoilAdd452		452		//13#�����¼�
#define		NCoilAdd453		453		//14#�����¼�
#define		NCoilAdd454		454		//15#�����¼�
#define		NCoilAdd455		455		//16#�����¼�
#define		NCoilAdd456		456		//17#�����¼�
#define		NCoilAdd457		457		//18#�����¼�����ͣ
//��������
#define 		NCoilAdd458		458		//����
#define 		NCoilAdd459		459		//����
#define 		NCoilAdd460		460		//����
#define 		NCoilAdd461		461		//����
//�̵�������
#define		NCoilAdd462		462		//1#�̵���
#define		NCoilAdd463		463		//2#�̵���
#define		NCoilAdd464		464		//3#�̵���
#define		NCoilAdd465		465		//4#�̵���
#define		NCoilAdd466		466		//5#�̵���
#define		NCoilAdd467		467		//6#�̵���
#define		NCoilAdd468		468		//7#�̵���
#define		NCoilAdd469		469		//8#�̵���
#define		NCoilAdd470		470		//9#�̵���
#define		NCoilAdd471		471		//10#�̵���
#define		NCoilAdd472		472		//11#�̵���
#define		NCoilAdd473		473		//12#�̵���
#define		NCoilAdd474		474		//13#�̵���
#define		NCoilAdd475		475		//14#�̵���
#define		NCoilAdd476		476		//15#�̵���
#define		NCoilAdd477		477		//16#�̵���
#define		NCoilAdd478		478		//17#�̵���
#define		NCoilAdd479		479		//18#�̵���
#define		NCoilAdd480		480		//19#�̵���
#define		NCoilAdd481		481		//20#�̵���
#define		NCoilAdd482		482		//21#�̵���
#define		NCoilAdd483		483		//22#�̵���
#define		NCoilAdd484		484		//23#�̵���
#define		NCoilAdd485		485		//24#�̵���
#define		NCoilAdd486		486		//25#�̵���
#define		NCoilAdd487		487		//26#�̵���
#define		NCoilAdd488		488		//27#�̵���
#define		NCoilAdd489		489		//28#�̵���
#define		NCoilAdd490		490		//29#�̵���
#define		NCoilAdd491		491		//30#�̵���
#define		NCoilAdd492		492		//31#�̵���
#define		NCoilAdd493		493		//32#�̵���
#define		NCoilAdd494		494		//33#�̵���
#define		NCoilAdd495		495		//34#�̵���
#define		NCoilAdd496		496		//35#�̵���
#define		NCoilAdd497		497		//36#�̵���
#define		NCoilAdd498		498		//37#�̵���
#define		NCoilAdd499		499		//38#�̵���
#define		NCoilAdd500		500		//39#�̵���
#define		NCoilAdd501		501		//40#�̵���

//�������
#define		NCoilAdd502		502		//#Pout1
#define		NCoilAdd503		503		//#Pout2
#define		NCoilAdd504		504		//#Pout3
#define		NCoilAdd505		505		//#Pout4
#define		NCoilAdd506		506		//#Pout5
#define		NCoilAdd507		507		//#Pout6
#define		NCoilAdd508		508		//#Pout7
#define		NCoilAdd509		509		//#Pout8
#define		NCoilAdd510		510		//#Pout9
#define		NCoilAdd511		511		//#Pout10
#define		NCoilAdd512		512		//#Pout11
#define		NCoilAdd513		513		//#Pout12
#define		NCoilAdd514		514		//#Pout13
#define		NCoilAdd515		515		//#Pout14
#define		NCoilAdd516		516		//#Pout15
#define		NCoilAdd517		517		//#Pout16

//�����ز���
#define    FreqCLK          168000000
#define    RISE             0		
#define    DROP             1		
#define    SUBDIVISION      3200
#define    COMPENSATE       10


#endif