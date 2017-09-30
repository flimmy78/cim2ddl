package com.znd.ei;

import static org.assertj.core.api.Assertions.assertThat;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import com.znd.ei.memdb.DbEntryOperations;

@RunWith(SpringRunner.class)
@SpringBootTest
public class ConfigTest {

	@Autowired
	DbEntryOperations pROps;
	
    @Test
    public void contexLoads() throws Exception {
        assertThat(pROps).isNotNull();
    }
    
    @Test
    public void tableCount() throws Exception {
    	assertThat(pROps.getTableNames()).contains("FState", "FStateFDev");
    }
}