package com.ledger.connections;

public interface LedgerSQL {
	
	boolean createSchema();
	boolean createAccount();
	boolean createTranaction();

}
