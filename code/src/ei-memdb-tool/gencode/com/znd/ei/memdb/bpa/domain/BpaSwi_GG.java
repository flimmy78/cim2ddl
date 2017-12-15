package com.znd.ei.memdb.bpa.domain;

import com.znd.ei.memdb.MemIndexable;

/**********************
*调速器和原动机组合模型表(GG)	*
***********************/
public class BpaSwi_GG  implements MemIndexable 
{
  private Integer id;
  
  private Integer memIndex;
  
  //卡类型
  private String CardKey;
  
  //发电机名称
  private String ACBus_Name;
  
  //发电机基准电压(kV)
  private Double ACBus_kV;
  
  //发电机识别码
  private Integer Gen_ID;
  
  //原动机最大输出功率(MW)
  private Double Gen_Pmax;
  
  //调差系数
  private Double GG_R;
  
  //控制时间
  private Double GG_T1;
  
  //水轮机恢复时间
  private Double GG_T2;
  
  //伺服机时间常数
  private Double GG_T3;
  
  //汽轮机阀时间常数
  private Double GG_T4;
  
  //水轮机1/2倍水锤效应时间常数/汽轮机再热器时间常数
  private Double GG_T5;
  
  //水轮机F=－2;对汽轮机F=再热器前轴容量/总的轴容量
  private Double GG_F;
  
  //数据库主键
  private String KeyName;
  
  //发电机母线索引
  private Integer BusPtr;
  
  //新增记录标记
  private Integer AppendTag;
  
  public Integer getId()
  {
    return id;
  }
    
  public void setId(Integer id)
  {
    this.id = id;
  }
    
  public Integer getMemIndex()
  {
    return memIndex;
  }
    
  public void setMemIndex(Integer memIndex)
  {
    this.memIndex = memIndex;
  }
    
  public String getCardKey()
  {
    return CardKey;
  }
    
  public void setCardKey(String CardKey)
  {
    this.CardKey = CardKey;
  }
    
  public String getACBus_Name()
  {
    return ACBus_Name;
  }
    
  public void setACBus_Name(String ACBus_Name)
  {
    this.ACBus_Name = ACBus_Name;
  }
    
  public Double getACBus_kV()
  {
    return ACBus_kV;
  }
    
  public void setACBus_kV(Double ACBus_kV)
  {
    this.ACBus_kV = ACBus_kV;
  }
    
  public Integer getGen_ID()
  {
    return Gen_ID;
  }
    
  public void setGen_ID(Integer Gen_ID)
  {
    this.Gen_ID = Gen_ID;
  }
    
  public Double getGen_Pmax()
  {
    return Gen_Pmax;
  }
    
  public void setGen_Pmax(Double Gen_Pmax)
  {
    this.Gen_Pmax = Gen_Pmax;
  }
    
  public Double getGG_R()
  {
    return GG_R;
  }
    
  public void setGG_R(Double GG_R)
  {
    this.GG_R = GG_R;
  }
    
  public Double getGG_T1()
  {
    return GG_T1;
  }
    
  public void setGG_T1(Double GG_T1)
  {
    this.GG_T1 = GG_T1;
  }
    
  public Double getGG_T2()
  {
    return GG_T2;
  }
    
  public void setGG_T2(Double GG_T2)
  {
    this.GG_T2 = GG_T2;
  }
    
  public Double getGG_T3()
  {
    return GG_T3;
  }
    
  public void setGG_T3(Double GG_T3)
  {
    this.GG_T3 = GG_T3;
  }
    
  public Double getGG_T4()
  {
    return GG_T4;
  }
    
  public void setGG_T4(Double GG_T4)
  {
    this.GG_T4 = GG_T4;
  }
    
  public Double getGG_T5()
  {
    return GG_T5;
  }
    
  public void setGG_T5(Double GG_T5)
  {
    this.GG_T5 = GG_T5;
  }
    
  public Double getGG_F()
  {
    return GG_F;
  }
    
  public void setGG_F(Double GG_F)
  {
    this.GG_F = GG_F;
  }
    
  public String getKeyName()
  {
    return KeyName;
  }
    
  public void setKeyName(String KeyName)
  {
    this.KeyName = KeyName;
  }
    
  public Integer getBusPtr()
  {
    return BusPtr;
  }
    
  public void setBusPtr(Integer BusPtr)
  {
    this.BusPtr = BusPtr;
  }
    
  public Integer getAppendTag()
  {
    return AppendTag;
  }
    
  public void setAppendTag(Integer AppendTag)
  {
    this.AppendTag = AppendTag;
  }
    
  public String toString()
  {
    return "BpaSwi_GG ["+"id = " + id
	 + ", memIndex = " + memIndex
	 + ", CardKey = " + CardKey
	 + ", ACBus_Name = " + ACBus_Name
	 + ", ACBus_kV = " + ACBus_kV
	 + ", Gen_ID = " + Gen_ID
	 + ", Gen_Pmax = " + Gen_Pmax
	 + ", GG_R = " + GG_R
	 + ", GG_T1 = " + GG_T1
	 + ", GG_T2 = " + GG_T2
	 + ", GG_T3 = " + GG_T3
	 + ", GG_T4 = " + GG_T4
	 + ", GG_T5 = " + GG_T5
	 + ", GG_F = " + GG_F
	 + ", KeyName = " + KeyName
	 + ", BusPtr = " + BusPtr
	 + ", AppendTag = " + AppendTag+"]";
  }
    
}
    