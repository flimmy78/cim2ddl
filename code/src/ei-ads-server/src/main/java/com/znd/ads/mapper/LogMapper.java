package com.znd.ads.mapper;

import org.apache.ibatis.annotations.Mapper;

import com.znd.ads.vo.LogInfo;

/**
 * 
 * @author wangheng
 * @date 2018年2月24日
 * @time 下午4:26:08
 * @type_name LogMapper
 */
@Mapper
public interface LogMapper {
    /**
     * 保存日志信息
     * @param log
     */
    void save(LogInfo log);
}