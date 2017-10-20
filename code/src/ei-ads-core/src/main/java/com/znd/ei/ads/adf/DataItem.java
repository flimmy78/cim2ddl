package com.znd.ei.ads.adf;

import com.znd.ei.ads.acp.IOOperations;

public abstract class DataItem<T> {
	protected IOOperations<T> operations;
	private String key;

	public String getKey() {
		return key;
	}

	public void setKey(String name) {
		this.key = name;
	}

	public IOOperations<T> getOperations() {
		return operations;
	}

	public void setOperations(IOOperations<T> operations) {
		this.operations = operations;
	}

	public abstract void clear();
	
	public abstract boolean canClear();

	public abstract boolean isEmpty();
}
