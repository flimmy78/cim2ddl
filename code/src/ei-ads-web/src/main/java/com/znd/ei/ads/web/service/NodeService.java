package com.znd.ei.ads.web.service;

import java.util.List;

import com.znd.ei.ads.web.vo.NodeInfo;

public interface NodeService {
    /**
     * 获取所有的节点
     * @return
     */
    List<NodeInfo> all();
    
    /**
     * 根据名称获取节点
     * @param name
     * @return
     */
    NodeInfo getByName(String name);
    
    /**
     * 根据名称获取节点
     * @param name
     * @return
     */
    NodeInfo getByUrl(String url);
    
    void save(NodeInfo info);
}
