package app.services;

import lombok.Getter;
import lombok.Setter;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

@Getter
@Setter
public class DatabaseService {
    private Connection connection;

    public DatabaseService() throws SQLException {
        setConnection(
                DriverManager.getConnection("jdbc:postgresql://localhost:5432/mtcg",
                "swe1user",
                "swe1pw"));
    }
}
