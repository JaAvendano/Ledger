package com.ledger.connections;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class SQLConnection {

	private static final String user = "root";
	private static final String pass = "password";
	private static final String url = "jdbc:mysql://127.0.0.1:3306";
	private static Connection con;
	

	// method connects to the mysql server.
	public static Connection getConnection() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			
			con = DriverManager.getConnection(url,user,pass);
		} catch (SQLException | ClassNotFoundException e) {
			e.printStackTrace();
		}
		return con;
	}
	
	//this method is exactly the same as the getConncetion method
	//it just has the extension at the end of the url to map it directly to the 
	//database.
	public static Connection routeToSchema() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			con = DriverManager.getConnection(url + "/my_ledger", user,pass);
		}catch(SQLException | ClassNotFoundException e) {
			e.printStackTrace();
		}
		return con;
	}
}
