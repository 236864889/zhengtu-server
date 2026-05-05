#ifndef _SCENEUSERMANAGER_H_
#define _SCENEUSERMANAGER_H_

#include "SceneUser.h"
#include "zUserManager.h"

class SceneUserManager:protected zUserManager
{
	protected:
		inline bool getUniqeID(DWORD& tempid);
		inline void putUniqeID(const DWORD& tempid);
		SceneUserManager();
		virtual ~SceneUserManager();
		static SceneUserManager *sum;
	public:
		static SceneUserManager &getMe();
		static void destroyMe();
		SceneUser * getUserByName( const char * name);
		SceneUser * getUserByID( DWORD id);
		SceneUser * getUserByTempID( DWORD tempid);
		SceneUser * getUserByNameOut( const char * name);
		SceneUser * getUserByIDOut( DWORD id);
		SceneUser * getUserByTempIDOut( DWORD tempid);
		bool addUser(SceneUser *user);
		void removeUser(SceneUser *user);
		void removeUserInOneScene(Scene *scene);
		void removeUserToHuangcheng(Scene *scene);
       ////////////////////////////////////////////////////////
	    void removeUserToWhere(Scene *fromScn, const char* toWhere);
	   //////////////////////////////////////////////////////
		void clearEmperorDare(Scene* scene);
		void setEmperorDare(Scene* scene);
		void removeUserByTask(SceneTask *task);
		void removeAllUser();
		DWORD countUserInOneScene(Scene *scene);
		DWORD countUserByTask(SceneTask *task);
		bool GuoJiaBiaoCheJiangLi1(Scene *scene, DWORD countryID);
		bool GuoJiaBiaoCheJiangLi2(Scene *scene, DWORD countryID);
		bool GuoJiaBiaoCheJiangLi3(Scene *scene, DWORD countryID);
		bool GuoJiaBiaoCheJiangLi4(Scene *scene, DWORD countryID);
		void countryTrans(DWORD dwCountryID, DWORD dwLevel);
		void setAntiAtt(DWORD dwType, DWORD dwFromRelationID, DWORD dwToRelationID);
		void enterWar(Cmd::Session::t_enterWar_SceneSession* cmd);
};


class SceneRecycleUserManager: public zEntryManager< zEntryID > 
{
	private:
		/**
		 ** \brief ʻ
		 **/
		zRWLock rwlock;
		static SceneRecycleUserManager *instance;
		SceneRecycleUserManager(){}
		~SceneRecycleUserManager(){}
	public:
		static SceneRecycleUserManager &getInstance();
		static void destroyInstance();
		SceneUser* getUserByID(DWORD id);
		void removeUser(SceneUser *user);
		bool addUser(zSceneEntry *user);
		bool canReg(DWORD id);
		void refresh();
		/**
		 ** \brief ÿûִ
		 ** \param exec ִнӿ
		 **/
		template <class YourUserEntry>
			bool execEveryUser(execEntry<YourUserEntry> &exec)
			{
				rwlock.rdlock();
				bool ret=execEveryEntry<>(exec);
				rwlock.unlock();
				return ret;
			}

};
#endif
