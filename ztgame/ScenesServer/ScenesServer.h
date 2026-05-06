/**
 * \file
 * \version  $Id: ScenesServer.h  $
 * \author  
 * \date 
 * \brief zebraĿϷ󲿷ݶڱʵ
 */

#ifndef _ScenesServer_h_
#define _ScenesServer_h_

#include "zSubNetService.h"
#include "Zebra.h"
#include "SceneManager.h"
#include "zMisc.h"

//sokeתӵļܺԵ
extern WORD trun_point_rate;
extern WORD trun_skill_rate;
/**
 * \brief 峡
 *
 * Ϸ󲿷ݶڱʵ<br>
 * ʹSingletonģʽ֤һֻһʵ
 *
 */
class ScenesService : public zSubNetService
{

	public:
		int writeBackTimer;
		//sky µһṹ
		struct txdy_info
		{
			int userid;//ɫid
			char username[MAX_NAMESIZE];//ɫ
			int saidian;//
			int jifen;//
		};
		txdy_info tianxia[32];

		//ͣ״̬
		bool fanwaigua;
		DWORD fanwaiguarongcuo;
		//Թϵͳ ʼ
		struct migong_info
		{
			int ceng;
			int password;
		};

		
		migong_info migong[25];
		
		SceneNpc * han_biaoche;//汉国完好镖车
		SceneNpc * han_biaoche2;//汉国破损镖车
		
		SceneNpc * chu_biaoche;//楚国完好镖车
		SceneNpc * chu_biaoche2;//楚国破损镖车
		SceneNpc * wei_biaoche;//魏国完好镖车
		SceneNpc * wei_biaoche2;//魏国破损镖车

		int hanbiaoche_type;
		int chubiaoche_type;
		int weibiaoche_type;
		///////////////////////////////
		//soke ־鱶
		int double_exp;
		///////////////////////////////
		bool double_drop;
		BYTE levelDoubleDrop;
		bool msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		BYTE countryPower[13];
		
		/**
		 * \brief 
		 *
		 */
		virtual ~ScenesService()
		{
			instance = NULL;

			//ر̳߳
			if (taskPool)
			{
				taskPool->final();
				SAFE_DELETE(taskPool);
			}
		}

		const int getPoolSize() const
		{
			if(taskPool)
			{
				return taskPool->getSize();
			}
			else
			{
				return 0;
			}
		}

		inline static void zcadfss()
		{
			ScenesService * aazz = &ScenesService::getInstance();
			delete aazz;
		}

		/**
		 * \brief Ψһʵ
		 *
		 * \return Ψһʵ
		 */
		static ScenesService &getInstance()
		{
			if (NULL == instance)
				instance = new ScenesService();

			return *instance;
		}

		/**
		 * \brief ͷΨһʵ
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		void reloadConfig();
		void checkAndReloadConfig();
		bool isSequeueTerminate() 
		{
			return taskPool == NULL;
		}

		//GM_logger
		static zLogger* gm_logger;
		//Ʒlog
		static zLogger* objlogger;
		//_logger
		static zLogger* wg_logger;

		inline static bool zcaaadfss();

		static Cmd::stChannelChatUserCmd * pStampData;
		static DWORD updateStampData();
	private:

		/**
		 * \brief Ψһʵָ
		 *
		 */
		static ScenesService *instance;
		/**
		 * \brief ¶ȡñ־
		 *
		 */
		static bool reload;

		zTCPTaskPool *taskPool;				/**< TCPӳصָ */

		/**
		 * \brief 캯
		 *
		 */
		ScenesService() : zSubNetService("", SCENESSERVER)
		{
			writeBackTimer = 0;

			taskPool = NULL;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
};

#endif

