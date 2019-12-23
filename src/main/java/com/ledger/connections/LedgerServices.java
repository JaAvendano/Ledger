package com.ledger.connections;

public class LedgerServices {

	SQLImp imp;
	/*
	 * this class meant for a convenient way to quickly make the desired
	 * calls to the database with as little code as possible. 
	 */
	
	public static void main(String[] args) {
		LedgerServices service = new LedgerServices();
		System.out.println("Schema executed: " + service.createSchema());
		System.out.println("Accounts Table: " + service.createAccount());
		System.out.println("Transaction Table: " + service.createTransaction());
		
	}
	
	public boolean createSchema() {
		imp = new SQLImp();
		return imp.createSchema();
	}
	
	public boolean createAccount() {
		imp = new SQLImp();
		return imp.createAccount();
		
	}
	
	public boolean createTransaction() {
		imp = new SQLImp();
		return imp.createTranaction();
	}
}
