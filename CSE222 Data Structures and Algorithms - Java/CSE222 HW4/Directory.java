import java.util.LinkedList;

public class Directory extends FileSystemElement {
    private LinkedList<FileSystemElement> children;

    public Directory(String name, FileSystemElement parent) {
        super(name, parent);
        this.children = new LinkedList<>();
    }

    public LinkedList<FileSystemElement> getChildren() {
        return children;
    }

    public void addChild(FileSystemElement child) {
        children.add(child);
    }

    public void removeChild(FileSystemElement child) {
        children.remove(child);
    }
}
