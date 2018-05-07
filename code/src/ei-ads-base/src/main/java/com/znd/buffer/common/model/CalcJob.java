package com.znd.buffer.common.model;

import java.util.Date;

import com.znd.ads.model.PRAdequacySetting;
import com.znd.buffer.annotation.Index;
import com.znd.ei.Utils;

public class CalcJob {
	@Index
	private String id;
	
	@Index
	private String name;
		
	private String desc;
		
	private String parameter;
	
	private Date start;
		
	private Date end;
	
	private Long elapse;
	
	
	public String getId() {
		return id;
	}
	
	public void setId(String id) {
		this.id = id;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}

	public String getDesc() {
		return desc;
	}

	public void setDesc(String desc) {
		this.desc = desc;
	}

	public String getParameter() {
		return parameter;
	}

	public void setParameter(String parameter) {
		this.parameter = parameter;
	}
	
	public static void setConfig(CalcJob job, PRAdequacySetting config) {
		job.setParameter(Utils.toJSon(config));
	}
	
	public static PRAdequacySetting getConfig(CalcJob job) {

		return Utils.toObject(job.getParameter(), PRAdequacySetting.class);
	}

	public Date getStart() {
		return start;
	}

	public void setStart(Date start) {
		this.start = start;
	}

	public Date getEnd() {
		return end;
	}

	public void setEnd(Date end) {
		this.end = end;
	}

	public Long getElapse() {
		return elapse;
	}

	public void setElapse(Long elapse) {
		this.elapse = elapse;
	}

	@Override
	public String toString() {
		return "CalcJob [id=" + id + ", name=" + name + ", desc=" + desc
				+ ", parameter=" + parameter + ", start=" + start + ", end="
				+ end + ", elapse=" + elapse + "]";
	}
}