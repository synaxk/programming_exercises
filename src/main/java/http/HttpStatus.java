package http;

import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

public enum HttpStatus {
    OK(200, "OK"),
    CREATED(201, "CREATED"),
    OK_EMPTY(204, "OK but empty"),
    BAD_REQUEST(400, "Bad Request"),
    UNAUTHORIZED(401, "Unauthorized"),
    FORBIDDEN(403, "Forbidden"),
    NOT_FOUND(404, "Not Found"),
    ALREADY_EXISTS(409, "Object already exists."),
    INTERNAL_SERVER_ERROR(500, "Internal Server Error");

    @Getter
    @Setter(AccessLevel.PRIVATE)
    private int code;
    @Getter
    @Setter(AccessLevel.PRIVATE)
    private String message;

    HttpStatus(int code, String message) {
        setCode(code);
        setMessage(message);
    }
}
