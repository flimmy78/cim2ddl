package com.znd.ei.ads.apl.reliability;

import io.netty.bootstrap.Bootstrap;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.DelimiterBasedFrameDecoder;
import io.netty.handler.codec.Delimiters;
import io.netty.handler.codec.string.StringDecoder;
import io.netty.handler.codec.string.StringEncoder;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.nio.charset.Charset;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.znd.ei.Utils;
import com.znd.ei.ads.adf.ListData;
import com.znd.ei.ads.adf.MapData;
import com.znd.ei.ads.apl.reliability.bean.RequestDataReady;
import com.znd.ei.ads.apl.reliability.bean.RequestEstimate;
import com.znd.ei.ads.apl.reliability.bean.RequestJobFinished;
import com.znd.ei.ads.apl.reliability.bean.ResponseEstimate;
import com.znd.ei.ads.apl.reliability.server.StateEstimateServer;
import com.znd.ei.ads.config.PRAdequacySetting;

@Component
public class StateEstimateProxyServer implements StateEstimateServer {
	private static final Logger logger = LoggerFactory
			.getLogger(StateEstimateProxyServer.class);


	//private AppExecuteBuilder appExec;

	@Autowired
	private StateEstimateProperties properties;

	
	public StateEstimateProxyServer() {

	}

	public StateEstimateProperties getProperties() {
		return properties;
	}


	public void setProperties(StateEstimateProperties properties) {
		this.properties = properties;
	}


	private ServerBootstrap localBootstrap;
	//private Channel responseChannel;

	private Bootstrap clientBootStrap;
	private NioEventLoopGroup clientEventGroup;
	
	
	private  ExecutorService threadPool = Executors.newCachedThreadPool();

	private Charset defaultCharset = Charset.forName("GBK");
	
//	private void stopJob() {
//		RequestJobFinished request = new RequestJobFinished();
//		String responseMsg = Utils.toJSon(request);
//		simpleSendMessage(responseMsg, null);
//	}
    
	
	/**
	 * 开始监听来自StateEstimate计算服务的消息
	 * @param buffer
	 * @param setting
	 * @param latch
	 * @param semaphore
	 * @throws InterruptedException
	 */
	private void startListen(ReliabilityNetBuffer buffer,
			PRAdequacySetting setting,
			CountDownLatch serverFinishedLatch, CountDownLatch listenStartLatch) throws InterruptedException {
		Semaphore availableWorkers = new Semaphore(properties.getServerThread(), true);

		EventLoopGroup bossGroup = new NioEventLoopGroup(); // (1)
		EventLoopGroup workerGroup = new NioEventLoopGroup();
		StateEstimateProxyServer server = this;
		ListData<RequestEstimate> taskList;
		MapData<String, ResponseEstimate> resultMap;
        taskList = buffer.getList(ReliabilityNetBuffer.ESTIMATE_TASK_LIST);
        resultMap = buffer.getMap(ReliabilityNetBuffer.ESTIMATE_RESULT_MAP);
        int taskSize =  taskList.getSize();
        AtomicBoolean closedFlag = new AtomicBoolean(false);      
        AtomicInteger currentTaskIndex = new AtomicInteger(0);
        
         ChannelInitializer<SocketChannel> chanelInit = new ChannelInitializer<SocketChannel>() { // 7
			@Override
			public void initChannel(SocketChannel ch)
					throws Exception {
		        ch.pipeline().addLast("framer", new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));
		        ch.pipeline().addLast("decoder", new StringDecoder(defaultCharset));
		        ch.pipeline().addLast("encoder", new StringEncoder(defaultCharset));		
		        StateEstimateResponseHandler handler = new StateEstimateResponseHandler(taskList, 
						resultMap, 
						taskSize, 
						currentTaskIndex, 
						server, 
						availableWorkers) {
					
					@Override
					public void closeParent() {
						if (closedFlag.getAndSet(true))
							return;
															

				        Timer timer = new Timer(true);
				        
				        TimerTask task = new TimerTask() {												
								@Override
								public void run() {
							    	//判断任务是否完成
									long resultSize = 0l;
									synchronized(resultMap) {
										resultSize = resultMap.size();
									}
									
									if (resultSize == taskSize) {
										System.out.println("Task is finished :"+taskList.getKey());
										RequestJobFinished request = new RequestJobFinished();
										String responseMsg = Utils.toJSon(request);
										simpleSendMessage(responseMsg, null);
										try {
											bossGroup.shutdownGracefully().sync();
											workerGroup.shutdownGracefully().sync();
											if (clientEventGroup != null)
											  clientEventGroup.shutdownGracefully().sync();
										} catch (InterruptedException e) {
											// TODO Auto-generated catch block
											e.printStackTrace();
										}									
										
										serverFinishedLatch.countDown();
										timer.cancel();
									}
									
								}
							};
							
						
						timer.schedule(task, 0, 1000l);
					}
				};		        
				ch.pipeline().addLast(handler);
			}
		};
        
		localBootstrap = new ServerBootstrap();
		localBootstrap
				.group(bossGroup, workerGroup)
				// 4
				.channel(NioServerSocketChannel.class)
				.childHandler(chanelInit);
		
		
		ChannelFuture f = localBootstrap.bind(properties.getListenPort()).sync(); // 8
		logger.info(StateEstimateProxyServer.class.getName()
				+ " started and listen on " + f.channel().localAddress());
		listenStartLatch.countDown();
		//responseChannel = f.channel();
		f.channel().closeFuture().sync(); // 9
		//responseChannel.close().sync();
		System.out.println("监听端口已关闭。");
	}
	
	private void startClient() {
	
		NioEventLoopGroup clientEventGroup = new NioEventLoopGroup();
	
		clientBootStrap = new Bootstrap();
		clientBootStrap.group(clientEventGroup)
         .channel(NioSocketChannel.class)
         .option(ChannelOption.TCP_NODELAY, true)
         .handler(new ChannelInitializer<SocketChannel>() {

             @Override
             public void initChannel(SocketChannel ch) throws Exception {
                 ChannelPipeline p = ch.pipeline();
            	 p.addLast("framer", new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));
                 p.addLast("decoder", new StringDecoder(defaultCharset));
                 p.addLast("encoder", new StringEncoder(defaultCharset));                    
//		                     p.addLast(new StateEstimateRequestHandler(msg));
             }
         });			
	}


	@Override
	public void exec(ReliabilityNetBuffer buffer, PRAdequacySetting setting) {
	//client
	   	setting.setMultiThread(properties.getServerThread());
	   	saveLog("PRAdequacySetting = \n"+Utils.toJSon(setting));
		CountDownLatch listenStartLatch = new CountDownLatch(1);
		CountDownLatch serverFinishedLatch = new CountDownLatch(1);
		
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					startListen(buffer, setting,serverFinishedLatch, listenStartLatch);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}).start();
		
		
//		new ChannelInitializer<SocketChannel>() {
//
//            @Override
//            public void initChannel(SocketChannel ch) throws Exception {
//                ChannelPipeline p = ch.pipeline();
//           	 p.addLast("framer", new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));
//                p.addLast("decoder", new StringDecoder(defaultCharset));
//                p.addLast("encoder", new StringEncoder(defaultCharset));                    
////		                     p.addLast(new StateEstimateRequestHandler(msg));
//            }
//        }
//		
//		startClient();
   	 
		RequestDataReady ready = new RequestDataReady();
		ready.setValue(String.valueOf(properties.getServerThread()));
		ready.getContent().setPRAdequacySetting(setting);
		
		String msg = Utils.toJSon(ready);
		simpleSendMessage(msg, listenStartLatch);
		System.out.println("Wait for server finished ...");
		try {
			serverFinishedLatch.await();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

	//private Byte sendFlag = new Byte((byte) 0);
	
	public void simpleSendMessage(String msg, CountDownLatch listenStartLatch) {
		if (listenStartLatch != null) {
			try {
				listenStartLatch.await();
			} catch (InterruptedException e2) {
				// TODO Auto-generated catch block
				e2.printStackTrace();
				logger.error("Fail to send message :"+msg);
				return;
			}
		}
		
		Socket ouputSocket = null;							
		//synchronized(sendFlag) {
		int reconnectCount = 0;
			while (ouputSocket == null) {

				reconnectCount++;
				try {
					ouputSocket = new Socket(properties.getServerIp(), properties.getServerPort());
					System.out.println(String.format(
							"Connected to server : ip:%s, port:%d",
							properties.getServerIp(), properties.getServerPort()));

				} catch (IOException e1) {
					System.err.println("Fail to connect to server.");
					if (reconnectCount > 3) {
						System.err.println("Fail to reconnect to server, after try "+ reconnectCount);
						return;
					}
					try {
						Thread.sleep(2000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}								
		//}
		System.out.println("Send data :\n" + msg);		
		try {
			BufferedWriter bw = new BufferedWriter(
					new OutputStreamWriter(
							ouputSocket.getOutputStream()));
			bw.write(msg);
			bw.flush();
			bw.close();
			ouputSocket.close();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			//e1.printStackTrace();
			System.out.println("Fail to send data, prepare to send data again:"+ e1.getMessage());			
		}
	}

	public void sendMessage(String msg, CountDownLatch latch) {
		if (latch != null) {
			try {
				latch.await();
			} catch (InterruptedException e2) {
				// TODO Auto-generated catch block
				e2.printStackTrace();
				logger.error("Fail to send message :"+msg);
				return;
			}
		}
		
		try {

	
			// Start the client.				
			ChannelFuture future = clientBootStrap.connect(properties.getServerIp(), properties.getServerPort()).sync();
			future.addListener(new ChannelFutureListener() {
				
				@Override
				public void operationComplete(ChannelFuture future) throws Exception {
					if (future.isSuccess()) {
						System.out.println("Connected to : ip = "+ properties.getServerIp()+", port = "+properties.getServerPort());
					} else {
						System.err.println("Fail to connected to : ip = "+ properties.getServerIp()+", port = "+properties.getServerPort());
					}
				}
			});


			future.channel().closeFuture().sync();
			System.out.println("Disconnect to : ip = "+ properties.getServerIp()+", port = "+properties.getServerPort());
								
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}


	
	private BufferedWriter bw;
	
	public void saveLog(String content) {
		if (bw == null) {
			try {
				File dir = new File(properties.getCachedDir());
				if (!dir.exists()) {
					dir.mkdirs();
				}
			OutputStreamWriter osw = new OutputStreamWriter(
					    new FileOutputStream(properties.getCachedDir()+"/estimateserver.log"), "UTF-8");
				bw = new BufferedWriter(osw);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
						
		}

		try {
			bw.write(content+"\n");
			bw.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}


	



}