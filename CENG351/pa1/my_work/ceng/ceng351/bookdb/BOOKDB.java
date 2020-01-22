package ceng.ceng351.bookdb;

import java.sql.*;
import java.util.ArrayList;

public class BOOKDB implements IBOOKDB {

    private static String user = "2310647";
    private static String password = "757abf54";
    private static String host = "144.122.71.65";
    private static String database = "db2310647";
    private static int port = 8084;

    private static Connection connection = null;

    @Override
    public void initialize()
    {
        String url = "jdbc:mysql://" + host + ":" + port + "/" + database;

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection =  DriverManager.getConnection(url, user, password);
        } catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public int createTables()
    {
        int count = 0;

        // author(author_id:int, author_name:varchar(60))
        String createAuthor = "create table if not exists author(" +
                "author_id int" +
                ", author_name varchar(60)" +
                ", primary key (author_id)" +
                ")";

        // publisher(publisher_id:int, publisher_name:varchar(50))
        String createPublisher = "create table if not exists publisher(" +
                "publisher_id int" +
                ", publisher_name varchar(50)" +
                ", primary key (publisher_id)" +
                ")";

        // book(isbn:char(13), book_name:varchar(120), publisher_id:int,
        // first_publish_year:char(4), page_count:int, category:varchar(25),
        // rating:float) REFERENCES publisher(publisher_id)
        String createBook = "create table if not exists book(" +
                "isbn char(13)" +
                ", book_name varchar(120)" +
                ", publisher_id int" +
                ", first_publish_year char(4)" +
                ", page_count int" +
                ", category varchar(25)" +
                ", rating float" +
                ", primary key (isbn)" +
                ", foreign key (publisher_id) references publisher(publisher_id)" +
                " on delete cascade on update cascade" +
                ")";

        // author_of(isbn:char(13),author_id:int) REFERENCES book(isbn) author(author_id)
        String createAuthorOf = "create table if not exists author_of(" +
                "isbn char(13)" +
                ", author_id int" +
                ", primary key (isbn, author_id)" +
                ", foreign key (isbn) references book(isbn)" +
                " on delete cascade on update cascade" +
                ", foreign key (author_id) references author(author_id)" +
                " on delete cascade on update cascade" +
                ")";

        // phw1(isbn:char(13), book_name:varchar(120), rating:float)
        String createPhw1 = "create table if not exists phw1(" +
                "isbn char(13)" +
                ", book_name varchar(120)" +
                ", rating float" +
                ", primary key (isbn)" + 
                ")";

        try {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(createAuthor);
            count++;

            statement.executeUpdate(createPublisher);
            count++;

            statement.executeUpdate(createBook);
            count++;

            statement.executeUpdate(createAuthorOf);
            count++;

            statement.executeUpdate(createPhw1);
            count++;

            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return count;
    }

    @Override
    public int dropTables()
    {
        int count = 0;

        String dropAuthor = "drop table if exists author";
        String dropPublisher = "drop table if exists publisher";
        String dropBook = "drop table if exists book";
        String dropAuthorOf = "drop table if exists author_of";
        String dropPhw1 = "drop table if exists phw1";

        try
        {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(dropAuthorOf);
            count++;

            statement.executeUpdate(dropAuthor);
            count++;

            statement.executeUpdate(dropBook);
            count++;

            statement.executeUpdate(dropPublisher);
            count++;

            statement.executeUpdate(dropPhw1);
            count++;

            statement.close();
        } catch(SQLException e)
        {
            e.printStackTrace();
        }
        
        return count;
    }

    @Override
    public int insertAuthor(Author[] authors)
    {
        int count = 0;

        try
        {
            PreparedStatement statement = this.connection.prepareStatement("insert into author values(?, ?)");
            for(int i = 0; i < authors.length; i++){
                Author author = authors[i];
                statement.setInt(1, author.getAuthor_id());
                statement.setString(2, author.getAuthor_name());

                statement.executeUpdate();
                count++;
            }

            statement.close();
        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        return count;
    }

    @Override
    public int insertBook(Book[] books)
    {
        int count = 0;
        
        // book(isbn:char(13), book_name:varchar(120), publisher_id:int,
        // first_publish_year:char(4), page_count:int, category:varchar(25),
        // rating:float) REFERENCES publisher(publisher_id)
        try
        {
            PreparedStatement statement = this.connection.prepareStatement(
                "insert into book values(?, ?, ?, ?, ?, ?, ?)");

            for(int i = 0; i < books.length; i++){
                Book book = books[i];
                statement.setString(1, book.getIsbn());
                statement.setString(2, book.getBook_name());
                statement.setInt(3, book.getPublisher_id());
                statement.setString(4, book.getFirst_publish_year());
                statement.setInt(5, book.getPage_count());
                statement.setString(6, book.getCategory());
                statement.setDouble(7, book.getRating());

                statement.executeUpdate();
                count++;
            }

            statement.close();
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        
        return count;
    }

    @Override
    public int insertPublisher(Publisher[] publishers)
    {
        // publisher(publisher_id:int, publisher_name:varchar(50))

        int count = 0;

        try
        {
            PreparedStatement statement = this.connection.prepareStatement("insert into publisher values(?, ?)");

            for(int i = 0; i < publishers.length; i++){
                Publisher publisher = publishers[i];
                statement.setInt(1, publisher.getPublisher_id());
                statement.setString(2, publisher.getPublisher_name());

                statement.executeUpdate();
                count++;
            }

            statement.close();
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }

        return count;
    }

    @Override
    public int insertAuthor_of(Author_of[] author_ofs)
    {
        // author_of(isbn:char(13),author_id:int) REFERENCES book(isbn) author(author_id)

        int count = 0;

        try
        {
            PreparedStatement statement = this.connection.prepareStatement("insert into author_of values(?, ?)");

            for(int i = 0; i < author_ofs.length; i++){
                Author_of author_of = author_ofs[i];
                statement.setString(1, author_of.getIsbn());
                statement.setInt(2, author_of.getAuthor_id());

                statement.executeUpdate();
                count++;
            }

            statement.close();
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }

        return count;
    }

    @Override
    public QueryResult.ResultQ1[] functionQ1()
    {
        ResultSet resultSet;
        ArrayList<QueryResult.ResultQ1> tmpList = new ArrayList<>();

        String query =
        "select b.isbn, b.first_publish_year, b.page_count, p.publisher_name" +
        " from book b, publisher p where b.publisher_id = p.publisher_id" +
        " and b.page_count = (select max(page_count) from book) order by b.isbn";

        try(Statement statement = this.connection.createStatement()){
            resultSet = statement.executeQuery(query);

            while(resultSet.next())
            {
                QueryResult.ResultQ1 entry =
                    new QueryResult.ResultQ1(resultSet.getString("isbn"),
                        resultSet.getString("first_publish_year"),
                        resultSet.getInt("page_count"),
                        resultSet.getString("publisher_name"));
                
                        tmpList.add(entry);
            }

        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        QueryResult.ResultQ1[] finalSet = new QueryResult.ResultQ1[tmpList.size()];
        
        return tmpList.toArray(finalSet);
    }

    @Override
    public QueryResult.ResultQ2[] functionQ2(int author_id1, int author_id2)
    {
        ResultSet resultSet;
        ArrayList<QueryResult.ResultQ2> tmpList = new ArrayList<>();

        String query =
        "select p.publisher_id, avg(b.page_count) as avgpc from" +
        " publisher p, book b where b.publisher_id=p.publisher_id and" +
        " p.publisher_id in (select b.publisher_id from book b" +
        ", author_of a1, author_of a2 where a1.isbn = a2.isbn and" +
        " a1.isbn = b.isbn and a1.author_id = " +
        Integer.toString(author_id1) +
        " and a2.author_id = " +
        Integer.toString(author_id2) +
        ") group by p.publisher_id order by p.publisher_id";

        try(Statement statement = this.connection.createStatement()){
            resultSet = statement.executeQuery(query);

            while(resultSet.next())
            {
                QueryResult.ResultQ2 entry =
                    new QueryResult.ResultQ2(
                        resultSet.getInt("publisher_id"),
                        resultSet.getDouble("avgpc"));
                
                        tmpList.add(entry);
            }

        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        QueryResult.ResultQ2[] finalSet = new QueryResult.ResultQ2[tmpList.size()];
        
        return tmpList.toArray(finalSet);
    }

    @Override
    public QueryResult.ResultQ3[] functionQ3(String author_name)
    {
        ResultSet resultSet;
        ArrayList<QueryResult.ResultQ3> tmpList = new ArrayList<>();

        String query =
        "select b.book_name, b.category, b.first_publish_year from book b, (" +
        "select min(first_publish_year) as minyear from book where isbn in (" +
        "select isbn from author a, author_of ao where a.author_name=\"" +
        author_name +
        "\" and ao.author_id = a.author_id)) as tmp" +
        " where b.first_publish_year = tmp.minyear";

        try(Statement statement = this.connection.createStatement()){
            resultSet = statement.executeQuery(query);

            while(resultSet.next())
            {
                QueryResult.ResultQ3 entry =
                    new QueryResult.ResultQ3(
                        resultSet.getString("book_name"),
                        resultSet.getString("category"),
                        resultSet.getString("first_publish_year"));
                
                        tmpList.add(entry);
            }

        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        QueryResult.ResultQ3[] finalSet = new QueryResult.ResultQ3[tmpList.size()];
        
        return tmpList.toArray(finalSet);
    }

    @Override
    public QueryResult.ResultQ4[] functionQ4()
    {
        ResultSet resultSet;
        ArrayList<QueryResult.ResultQ4> tmpList = new ArrayList<>();

        String query =
        "select distinct publisher_id, category from book where" +
        " publisher_id in (select b.publisher_id from book b, publisher p" +
        " where p.publisher_id = b.publisher_id and p.publisher_name" + 
        " like '% % %' group by publisher_id having count(*) > 2 and" +
        " avg(rating) > 3) order by publisher_id, category";

        try(Statement statement = this.connection.createStatement()){
            resultSet = statement.executeQuery(query);

            while(resultSet.next())
            {
                QueryResult.ResultQ4 entry =
                    new QueryResult.ResultQ4(
                        resultSet.getInt("publisher_id"),
                        resultSet.getString("category"));
                
                        tmpList.add(entry);
            }

        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        QueryResult.ResultQ4[] finalSet = new QueryResult.ResultQ4[tmpList.size()];
        
        return tmpList.toArray(finalSet);
    }

    @Override
    public QueryResult.ResultQ5[] functionQ5(int author_id)
    {
        ResultSet resultSet;
        ArrayList<QueryResult.ResultQ5> tmpList = new ArrayList<>();

        String query =
        "select a.author_id, a.author_name from author a where not exists" +
        "(select p.publisher_id from publisher p, author_of ao, book b" +
        " where ao.author_id = " +
        Integer.toString(author_id) +
        " and ao.isbn = b.isbn and b.publisher_id = p.publisher_id and" +
        " p.publisher_id not in (select p.publisher_id from publisher p," +
        " author_of ao, book b where ao.author_id = a.author_id and" +
        " ao.isbn = b.isbn and b.publisher_id = p.publisher_id)) order by" +
        " a.author_id";

        try(Statement statement = this.connection.createStatement()){
            resultSet = statement.executeQuery(query);

            while(resultSet.next())
            {
                QueryResult.ResultQ5 entry =
                    new QueryResult.ResultQ5(
                        resultSet.getInt("author_id"),
                        resultSet.getString("author_name"));
                
                        tmpList.add(entry);
            }

        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        QueryResult.ResultQ5[] finalSet = new QueryResult.ResultQ5[tmpList.size()];
        
        return tmpList.toArray(finalSet);
    }

    @Override
    public QueryResult.ResultQ6[] functionQ6()
    {
        ResultSet resultSet;
        ArrayList<QueryResult.ResultQ6> tmpList = new ArrayList<>();

        String query =
        "select a.author_id, b.isbn from author a, book b, author_of ao" +
        " where a.author_id = ao.author_id and ao.isbn = b.isbn and a.author_id in" +
        "(select distinct a.author_id from book b, publisher p" +
        ", author_of a where" +
        " a.isbn = b.isbn and b.publisher_id = p.publisher_id and" +
        " not exists(select * from book b, author_of ao where" +
        " p.publisher_id = b.publisher_id and b.isbn = ao.isbn and" +
        " a.author_id <> ao.author_id)) order by a.author_id, b.isbn";

        try(Statement statement = this.connection.createStatement()){
            resultSet = statement.executeQuery(query);

            while(resultSet.next())
            {
                QueryResult.ResultQ6 entry =
                    new QueryResult.ResultQ6(
                        resultSet.getInt("author_id"),
                        resultSet.getString("isbn"));
                
                        tmpList.add(entry);
            }

        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        QueryResult.ResultQ6[] finalSet = new QueryResult.ResultQ6[tmpList.size()];
        
        return tmpList.toArray(finalSet);
    }

    @Override
    public QueryResult.ResultQ7[] functionQ7(double rating)
    {
        ResultSet resultSet;
        ArrayList<QueryResult.ResultQ7> tmpList = new ArrayList<>();

        String query =
        "select p.publisher_id, p.publisher_name from publisher p where" +
        " 1 < (select count(*) from book b where" +
        " b.publisher_id = p.publisher_id group by category having" +
        " category = 'Roman') and " +
        Double.toString(rating) +
        " < (select avg(b.rating) from book b where" +
        " b.publisher_id = p.publisher_id) order by p.publisher_id";

        try(Statement statement = this.connection.createStatement()){
            resultSet = statement.executeQuery(query);

            while(resultSet.next())
            {
                QueryResult.ResultQ7 entry =
                    new QueryResult.ResultQ7(
                        resultSet.getInt("publisher_id"),
                        resultSet.getString("publisher_name"));
                
                        tmpList.add(entry);
            }

        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        QueryResult.ResultQ7[] finalSet = new QueryResult.ResultQ7[tmpList.size()];
        
        return tmpList.toArray(finalSet);
    }

    @Override
    public QueryResult.ResultQ8[] functionQ8()
    {
        ResultSet resultSet;
        ArrayList<QueryResult.ResultQ8> tmpList = new ArrayList<>();

        String queryInsert =
        "insert into phw1 select b.isbn, b.book_name, b.rating from" +
        " book b, (select book_name, min(rating) as minrate from book" +
        " group by book_name having 1 < count(*)) as tmp where" +
        " b.book_name = tmp.book_name and b.rating = tmp.minrate";

        String query =
        "select * from phw1 order by isbn";

        try(Statement statement = this.connection.createStatement()){
            statement.executeUpdate(queryInsert);
            resultSet = statement.executeQuery(query);

            while(resultSet.next())
            {
                QueryResult.ResultQ8 entry =
                    new QueryResult.ResultQ8(
                        resultSet.getString("isbn"),
                        resultSet.getString("book_name"),
                        resultSet.getDouble("rating"));
                
                        tmpList.add(entry);
            }

        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        QueryResult.ResultQ8[] finalSet = new QueryResult.ResultQ8[tmpList.size()];
        
        return tmpList.toArray(finalSet);
    }

    @Override
    public double functionQ9(String keyword)
    {
        ResultSet resultSet;
        double res = 0;

        String queryUpdate =
        "update book set rating = rating + 1 where" +
        " book_name like \"%" + keyword + "%\" and rating <= 4";

        String query =
        "select sum(rating) as sumrate from book";

        try(Statement statement = this.connection.createStatement()){
            statement.executeUpdate(queryUpdate);
            resultSet = statement.executeQuery(query);

            resultSet.next();
            res = resultSet.getDouble("sumrate");
        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        return res;
    }

    @Override
    public int function10()
    {
        ResultSet resultSet;
        int res = 0;

        String queryUpdate =
        "delete from publisher where publisher_id not in (select distinct publisher_id from book)";

        String query =
        "select count(*) as countrows from publisher";

        try(Statement statement = this.connection.createStatement()){
            statement.executeUpdate(queryUpdate);
            resultSet = statement.executeQuery(query);

            resultSet.next();
            res = resultSet.getInt("countrows");
        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        return res;
    }
}
