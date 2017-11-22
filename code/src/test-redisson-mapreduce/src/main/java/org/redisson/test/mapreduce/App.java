package org.redisson.test.mapreduce;


import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import org.redisson.RedissonNode;
import org.redisson.api.RBucket;
import org.redisson.api.RExecutorService;
import org.redisson.api.RLiveObjectService;
import org.redisson.api.RMap;
import org.redisson.api.RedissonClient;
import org.redisson.api.mapreduce.RMapReduce;
import org.redisson.config.Config;
import org.redisson.config.RedissonNodeConfig;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;

/**
 * Hello world!
 *
 */
@SpringBootApplication
public class App 
{
	static class TestJson {
		
		int value1;
		String value2;
		double value3;
		public int getValue1() {
			return value1;
		}
		public void setValue1(int value1) {
			this.value1 = value1;
		}
		public String getValue2() {
			return value2;
		}
		public void setValue2(String value2) {
			this.value2 = value2;
		}
		public double getValue3() {
			return value3;
		}
		public void setValue3(double value3) {
			this.value3 = value3;
		}
		
		
	}

    @SuppressWarnings("unused")
	public static void main( String[] args )
    {
    	ApplicationContext ctx = (ApplicationContext) SpringApplication.run(App.class, args);
    	//testNodeStart(ctx);
    	//testMapReduce(ctx);
    	//testExecutorService(ctx);
    	//System.exit(SpringApplication.exit(ctx));
    	testLiveObject(ctx);
    }
    
    public static void testNodeStart(ApplicationContext ctx) {
    	RedissonNodeConfig config = ctx.getBean(RedissonNodeConfig.class);
  	    
  	 //   RedissonNodeConfig nodeConfig = new RedissonNodeConfig(config);
  	    
	    RedissonNode node = RedissonNode.create(config);
	    node.start();
    }
    
    public static void testMapReduce(ApplicationContext ctx) {
        //System.out.println( "Hello World!" );
  		


  		RedissonClient redisson = ctx.getBean(RedissonClient.class);
  		
  		RBucket<TestClass> b = redisson.getBucket("test1");
  		//b.set(new TestClass());
  		TestClass c = b.get();
  	    RMap<String, String> map = redisson.getMap("wordsMap");
  	    //map.clear();
  	    map.put("line1", "Alice was beginning to get very tired"); 
  	    map.put("line2", "of sitting by her sister on the bank and");
  	    map.put("line3", "of having nothing to do once or twice she");
  	    map.put("line4", "had peeped into the book her sister was reading");
  	    map.put("line5", "but it had no pictures or conversations in it");
  	    map.put("line6", "and what is the use of a book");
  	    map.put("line7", "thought Alice without pictures or conversation");
  	     Set<Entry<String, String>> s = map.entrySet();
  	    for (Entry<String, String> e: s) {
  	    	System.out.println("key = "+e.getKey()+", value = "+e.getValue());
  	    }
  	    Config config = ctx.getBean(Config.class);
  	    
  	    RedissonNodeConfig nodeConfig = new RedissonNodeConfig(config);
  	   // redisson.getExecutorService(RExecutorService.MAPREDUCE_NAME).registerWorkers(5);
  	   // String serviceName = "test1";
  	   // System.out.println(serviceName);
  	   // RScheduledExecutorService es = redisson.getExecutorService(serviceName);
  	    //nodeConfig.setExecutor(es);
  	   // es.MAPREDUCE_NAME;
  	    
  	    //RScheduledExecutorService mes = redisson.getExecutorService(RExecutorService.MAPREDUCE_NAME);
  	    //nodeConfig.setMapReduceWorkers(5);
  	    
//  	   int n =  es.countActiveWorkers();
//  	   boolean bt = es.isTerminated();
//  	   boolean bs = es.isShutdown();
  	   

//  	    Map<String, Integer> workers = new HashMap<String, Integer>();
//  	    workers.put("test1", 2);
//  	    nodeConfig.setExecutorServiceWorkers(workers);
//  	    RedissonNode node = RedissonNode.create(nodeConfig);
//  	    node.start();
  	    
  	    int n = nodeConfig.getMapReduceWorkers();
  	    RMapReduce<String, String, String, Integer> mapReduce
  	             = map.<String, Integer>mapReduce()
  	                  .mapper(new WordMapper())
  	                  .reducer(new WordReducer()).timeout(1, TimeUnit.MILLISECONDS);
  	    
  	    
  	    System.out.println("Start statistic ...");

  	    // 统计词频
  	    Map<String, Integer> mapToNumber = mapReduce.execute();
  	   
  	    System.out.println("统计词频");
  	    Set<Entry<String, Integer>> mns = mapToNumber.entrySet();
  	    for (Entry<String, Integer> e : mns) {
  	    	System.out.println("key = "+e.getKey()+", count = "+e.getValue());	
  	    }
  	    

  	    // 统计字数
  	    Integer totalWordsAmount = mapReduce.execute(new WordCollator());
  	    
  		System.out.println("totalWordsAmount= "+totalWordsAmount);
  		
    }
    
   public static void testExecutorService(ApplicationContext ctx) {
  		RedissonClient redisson = ctx.getBean(RedissonClient.class);
  		
  		
    	RExecutorService executorService = redisson.getExecutorService("myExecutor");
    	Future<Long> future = executorService.submit(new CallableTask());
    	try {
			Long result = future.get();
			System.out.println("Result :"+result);
		} catch (InterruptedException | ExecutionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
   
   
   public static void testLiveObject(ApplicationContext ctx) {
	   RedissonClient redisson = ctx.getBean(RedissonClient.class);
	 //首先获取服务实例
	   RLiveObjectService service = redisson.getLiveObjectService();
	   
	 //通过服务实例构造RLO实例
	   String id = "liveObjectId";
	   MyLiveObject standardObject1 = service.<MyLiveObject, String>get(MyLiveObject.class, id);
	   if (standardObject1 == null) {
		   standardObject1 = new MyLiveObject();
		   standardObject1.setName(id);		   
		   standardObject1 = service.<MyLiveObject>persist(standardObject1);
	   }
	   
	   Map<String, String> values = standardObject1.getValues();
	   values.put("key1", "value1");
	   values.put("key2", "value2");
	   values.put("key3", "value3");
	   //standardObject1.setValues(values);
   }
}
