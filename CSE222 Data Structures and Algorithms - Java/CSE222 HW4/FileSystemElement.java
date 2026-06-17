import java.sql.Timestamp;

public abstract class FileSystemElement {

    protected String name;
    protected Timestamp dateCreated;
    protected FileSystemElement parent;

    public FileSystemElement(String name, FileSystemElement parent) {
        this.name = name;
        this.dateCreated = new Timestamp(System.currentTimeMillis());
        this.parent = parent;
    }

    public String getName() {
        return name;
    }

    public Timestamp getDateCreated() {
        return dateCreated;
    }

    public FileSystemElement getParent() {
        return parent;
    }

    public void setName(String name) {
        this.name = name;
    }

    public FileSystemElement setParent(FileSystemElement parent) {
        return this.parent = parent;
    }

}