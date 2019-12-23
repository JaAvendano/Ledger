package com.ledger.connections;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class SQLImp extends SQLConnection implements LedgerSQL {
	
//class defines all the methods from the LedgerSQL interface for implementation.
	Connection con = null;
	PreparedStatement stm = null;
	Statement state = null;

	/*
	 * methods are all essentially the same, the only thing that's
	 * changing is the sql string
	 */
	@Override
	public boolean createSchema() 
	{
		try 
		{
			/*
			 * Here 3 queries are sent at once using the addBatch method and then executing with the executeBatch method.
			 * Statement was used here because it's executing sequentially by line.
			 * Generally it's preferable to use PreparedStatement instead of Statement because it
			 * offers a little more security against SQL injection but, i looked like PreparedStatement wasn't executing sequentially.
			 * I'm thinking that had more to do with the way I had the code written though and not necessarily the sequential execution.
			 */
			con = getConnection();//getConncetion method from SQLConncetion class
			/*
			 * I tried to use as much of you code as I could but ther's also a CREATE IF NOT EXIST clause you could
			 * use if you don't want to drop the schema altogether.
			 */
			String sqlDropSchema =  "DROP SCHEMA IF EXISTS my_ledger";//schema and database are used synonymously in mysql.
			String sqlCreateSchema = "CREATE SCHEMA my_ledger";
			state = con.createStatement();
			state.addBatch(sqlDropSchema);
			System.out.println("Schema dropped");// remove these 
			state.addBatch(sqlCreateSchema);
			System.out.println("New Schema added");
			//state.addBatch(useNewSchema);
			//System.out.println("New Schema picked");
			state.executeBatch();
			return true;
		}
		catch(SQLException e) 
		{
			e.printStackTrace();
			return false;
		}
		finally 
		{
			closeConn();//closing all connections
		}
		
	}

	@Override
	public boolean createAccount() 
	{
		try 
		{
			/*
			 * Here I used the route to schema method from the SQLConnection class
			 * its the exact same thing as the getConncetion method SQLConnection class.
			 * The only difference is the extension that's used on the URL string.
			 * Essentially it's a Rest call where the string extension maps it directly to the my_ledger database
			 * instead of having to specify which database to use each time.
			 */
			con = routeToSchema();
			String sql = "CREATE TABLE accounts "
					+ "(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, "
					+ "institution VARCHAR(100) NOT NULL, "
					+ "type INT NOT NULL, "
					+ "name VARCHAR(50) NOT NULL, "
					+ "active INT NOT NULL, "
					+ "balance DECIMAL(9,2) NOT NULL)"
					+ " ENGINE = InnoDB DEFAULT CHARACTER SET = utf8";
			stm = con.prepareStatement(sql);
			stm.execute();//this returns a boolean value but for whatever reason their values are always opposite of what they are.
			return true;
		}
		catch(SQLException e) 
		{
			e.printStackTrace();
			return false;
		}
		finally 
		{
			closeConn();
		}
	}

	@Override
	public boolean createTranaction() 
	{
		try 
		{
			//same logic as above.
			con = routeToSchema();
			String sql = "CREATE TABLE transactions ("
					+ "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
					+ "acc_id INT NOT NULL,"
					+ "status VARCHAR(1),"
					+ "day INT NOT NULL,"
					+ "month INT NOT NULL,"
					+ "year INT NOT NULL,"
					+ "name VARCHAR(50) NOT NULL,"
					+ "method INT NOT NULL,"
					+ "cat INT NOT NULL,"
					+ "amount DECIMAL(15,2) NOT NULL,"
					+ "balance DECIMAL(15,2) NOT NULL)"
					+ " ENGINE = InnoDB DEFAULT CHARACTER SET = utf8";
			stm = con.prepareStatement(sql);
			stm.execute();
			return true;
		}
		catch(SQLException e) 
		{
			e.printStackTrace();
			return false;
		}
		finally 
		{
			closeConn();
		}
	}
	
	
	//close all connections to avoid memory leaks
	public void closeConn() 
	{
		try 
		{
			if (stm != null) 
			{
				stm.close();
			} 
			if (con != null) 
			{
				con.close();
			}
			System.out.println("Connection Closed");
		}
		catch (SQLException e) 
		{
			System.out.println("Connection not properly closed!");
		}
	}

}
