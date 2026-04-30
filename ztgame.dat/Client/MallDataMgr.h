#pragma once

// 商品区域分类
enum WAREREGION_CLASS 
{
	WAREREGION_DISCOUNT, //	商品折扣区
	WAREREGION_HOT,		 // 热卖商品区
	WAREREGION_PROPS,	 // 便利道具区
	WAREREGION_RIDE,	 // 坐骑系列区
	WAREREGION_MATERIAL, // 精品材料区
	WAREREGION_OTHER,	 // 其他商品区
	WAREREGION_VIP,      // VIP专区 //by=>friday
	WAREREGION_FASHION,  // 时装专区 //by=>friday
	WAREREGION_WEAPON,   // 武器专区 //by=>friday
	WAREREGION_PET,      // 宠物专区 //by=>friday
	WAREREGION_COUNT,
};

class CMallDataMgr
{
	typedef std::vector<unsigned int> WARES;
	typedef std::map<WAREREGION_CLASS, WARES> TMAPWARES;

public:
	CMallDataMgr();
	~CMallDataMgr();

	unsigned int GetWaresNum(WAREREGION_CLASS eRegion); // 返回某区商品的数量
	unsigned int GetWareID(WAREREGION_CLASS eRegion, unsigned int index); // 根据索引得到某区商品的ID

private:
	void LoadWares();

private:
	TMAPWARES m_mapWares;
};

CMallDataMgr& GetMallDataMgr();
