package com.znd.ei.memdb.bpa.domain;

import com.znd.ei.memdb.MemIndexable;

/**********************
*发电负荷调整表(P)	*
***********************/
public class BpaDat_P  implements MemIndexable 
{
  private Integer id;
  
  private Integer memIndex;
  
  //卡类型
  private String CardKey;
  
  //所有者代码
  private String P_Zone;
  
  //负荷有功修改因子
  private Double P_LoadPFactor;
  
  //负荷无功修改因子
  private Double P_LoadQFactor;
  
  //发电出力有功修改因子
  private Double P_GenPFactor;
  
  //发电出力无功修改因子
  private Double P_GenQFactor;
  
  //状态
  private Integer P_Status;
  
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
    
  public String getP_Zone()
  {
    return P_Zone;
  }
    
  public void setP_Zone(String P_Zone)
  {
    this.P_Zone = P_Zone;
  }
    
  public Double getP_LoadPFactor()
  {
    return P_LoadPFactor;
  }
    
  public void setP_LoadPFactor(Double P_LoadPFactor)
  {
    this.P_LoadPFactor = P_LoadPFactor;
  }
    
  public Double getP_LoadQFactor()
  {
    return P_LoadQFactor;
  }
    
  public void setP_LoadQFactor(Double P_LoadQFactor)
  {
    this.P_LoadQFactor = P_LoadQFactor;
  }
    
  public Double getP_GenPFactor()
  {
    return P_GenPFactor;
  }
    
  public void setP_GenPFactor(Double P_GenPFactor)
  {
    this.P_GenPFactor = P_GenPFactor;
  }
    
  public Double getP_GenQFactor()
  {
    return P_GenQFactor;
  }
    
  public void setP_GenQFactor(Double P_GenQFactor)
  {
    this.P_GenQFactor = P_GenQFactor;
  }
    
  public Integer getP_Status()
  {
    return P_Status;
  }
    
  public void setP_Status(Integer P_Status)
  {
    this.P_Status = P_Status;
  }
    
  public String toString()
  {
    return "BpaDat_P ["+"id = " + id
	 + ", memIndex = " + memIndex
	 + ", CardKey = " + CardKey
	 + ", P_Zone = " + P_Zone
	 + ", P_LoadPFactor = " + P_LoadPFactor
	 + ", P_LoadQFactor = " + P_LoadQFactor
	 + ", P_GenPFactor = " + P_GenPFactor
	 + ", P_GenQFactor = " + P_GenQFactor
	 + ", P_Status = " + P_Status+"]";
  }
    
}
    
