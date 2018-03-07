package com.znd.ads.server.config;

import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.znd.ei.ads.buffer.BufferConfig;
import com.znd.ei.ads.buffer.BufferFactory;
import com.znd.ei.ads.buffer.BufferFactoryBuilder;
import com.znd.ei.ads.buffer.TableMeta;

@Configuration
@EnableAutoConfiguration
public class Config {
	@Bean
	public BufferConfig bufferConfig(DabaBusProperties dabaBusProperties) {
		
		BufferConfig config = new BufferConfig();
			config.setId(dabaBusProperties.getId());
			config.setName(dabaBusProperties.getName());
			config.setAlias(dabaBusProperties.getAlias());
			config.setAppName(BufferFactoryBuilder.DEFAULT_APPNAME);
			config.setCreateFlag(BufferConfig.UPDATE);
			config.setTableMetas(dabaBusProperties.getTables().toArray(new TableMeta[0]));
			return config;
	}
	
	
	@Bean 
	public BufferFactory bufferFactory(BufferConfig config) {
		BufferFactoryBuilder b = new BufferFactoryBuilder();
		return b.build(config);
	}
}