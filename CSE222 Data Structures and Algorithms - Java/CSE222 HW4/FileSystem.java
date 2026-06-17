import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Scanner;
import java.security.Timestamp;

public class FileSystem {
    private Directory root;

    public FileSystem() {
        this.root = new Directory("root", null);
    }

    public Directory getRoot() {
        return root;
    }

    /**
     * Helper function to get the remaining path and return it for changing the directory mehods and etc.
     * @param names
     * @return
     */
    public String getRemainingPath(ArrayList<String> names){
        String remainingPath="";
        for(int i=1;i<names.size();i++)
            remainingPath+="/" + names.get(i);
        return remainingPath;
    }

    /**
     * Changing the current directory by first taking the newPath and splitting it with '/' to an array then to ArrayList for removing and simplicity.
     * Then going to the first directory in the path by checking the name of the current child directory and if it is found changing our current directory
     * and calling the method again with our new currentDirectory location it continues till the last directory in the path to go to.  
     * @param currentDirectory
     * @param newPath
     * @return Last directory in the path.
     */
    public Directory ChangeDirectory(Directory currentDirectory,String newPath){
        if(newPath.equals("/root"))
            return root;
        String[] names = newPath.split("/");
        if(names.length==1 || names[names.length-1].equals(currentDirectory.getName())){ // checking if we are in the last directory in the path.
            return currentDirectory;
        }
        ArrayList<String> main = new ArrayList<>();
        for(String n : names){
            if(!n.isEmpty() ){
                if( !n.equals("root") )
                    main.add(n);
            }
        }

        for(FileSystemElement f : currentDirectory.getChildren()){
            if(f.getName().equals(main.get(0))){
                currentDirectory = (Directory)f;
                newPath=getRemainingPath(main);
                currentDirectory = ChangeDirectory(currentDirectory, newPath);
            }
        }
        return currentDirectory;
    }

    /**
     * Listing the contents of the current directory using an enhanced loop 
     * it checks if it is a file or directory using 'instanceof' then prints it accordingly.
     * @param currentDirectory
     * @param currentPath
     */
    public void ListContents(Directory currentDirectory,String currentPath){
        for(FileSystemElement files : currentDirectory.getChildren() ) {
            if(files instanceof File)
                System.out.println(files.getName());
            else if(files instanceof Directory)  
                System.out.println("* " + files.getName() + "/");
        }
    }


    

    /**
     * Creating a new directory or file after taking the type and name from the user. According to the type ;
     * Creating the new file/directory using the add method of directory and setting its parent to our current directory using the constructor of File.
     * @param currentDirectory
     * @param currentPath
     */
    public void CreateFileorDirectory(Directory currentDirectory,String currentPath){
        Scanner scanner = new Scanner(System.in);
        System.out.println("Current directory: " +  currentPath);
        System.out.print("Create file or directory (f/d): ");
        String fileType = scanner.nextLine();
        if (fileType.equals("f")) {
            System.out.print("Enter name for new file: ");
            String fileName = scanner.nextLine();
            File newFile = new File(fileName,currentDirectory); 
            currentDirectory.addChild(newFile);
            System.out.println("File created: " + fileName);
        } 
        
        else if (fileType.equals("d")) {
            System.out.print("Enter name for new directory: ");
            String directoryName = scanner.nextLine();
            Directory newDirectory = new Directory(directoryName,currentDirectory); 
            currentDirectory.addChild(newDirectory);
            System.out.println("Directory created: " + directoryName + "/");    
        } 
        else {
            System.out.println("Invalid input. Please enter 'f' for file or 'd' for directory.Returning to menu...");
        }
    }

    /**
     * Using the children of the current directory we are first checking if the first child is the file to be deleted if it is we delete it and set its parent to null.
     * If not we create a new linked list for children which does not consist of the already visited child which is the first one then
     * calling our method recursively by using this new children linked list until we find the file to be deleted or size of the linked list is 1. 
     * @param currentDirectory
     * @param fileName
     * @param child
     */
    public void DeleteFileorDirectory(Directory currentDirectory,String fileName,LinkedList<FileSystemElement> child){ // recursive normally
        if(child.get(0).getName().equals(fileName)){ // if the file to be deleted is found.
            if(child.get(0) instanceof File){
                System.out.println("File deleted : " + fileName);
            }
            else if(child.get(0) instanceof Directory){
                System.out.println("Directory deleted : " + fileName + "/");
            }
            child.get(0).setParent(null);
            currentDirectory.removeChild(child.get(0));
        }
        else{
            if(child.size()==1) // if not found which means there is no file with fileName in the system.
                return;
            LinkedList<FileSystemElement> child2 =new LinkedList<>();
            for(int i=1;i<child.size();i++){
                child2.add(child.get(i));
            }
            DeleteFileorDirectory(currentDirectory, fileName, child2);
        }
    }
    
    /**
     * Moving the directory by first finding the file to be moved then removing the file to be moved from the current directory with index remove.
     * After that, using our change directory method we change our current directory to the path that the file will be moved.
     * After going to the path the file is going to be moved we add the file to be moved to that directory and printing if it was successful. 
     * @param currentDirectory
     * @param currentPath
     */
    public void MoveFileorDirectory(Directory currentDirectory,String currentPath){
        Scanner scanner = new Scanner(System.in);
        System.out.println("Current directory: " + currentPath);
        System.out.print("Enter name of file/directory to move: ");
        String moveName = scanner.nextLine();
        System.out.print("Enter new directory path: ");
        String movePath = scanner.nextLine();
        boolean done=false;

        FileSystemElement moveElement=null;
        for(int i=0;i<currentDirectory.getChildren().size();i++){
            if(currentDirectory.getChildren().get(i).getName().equals(moveName)){
                System.out.println("Found!");
                done=true;
                moveElement = currentDirectory.getChildren().remove(i);
                currentDirectory.removeChild(moveElement);
                break;
            }
        }
        
        // after deleting the file from the current path going to the move path and adding the deleted file to the move path directory.
        currentDirectory=ChangeDirectory(root,movePath);
        currentDirectory.addChild(moveElement);
        // go to path and add moveElement
        
        if(done)
            System.out.println("File/directory moved: " + moveName + " to " + movePath);
    }   

    /**
     * Searcing the given file in our file management system by first checking if the file is in the children directory if it is we are printing the path 
     * If it is not when we find a FileSystemElement which is an instanceof Directory then
     * We call the method recursively by changing the current directory to that Directory and it continues until
     * All the directories have been searched or the file is found.
     * @param dir
     * @param searchQuery
     * @param path
     */
    public boolean SearchForFiles(Directory dir, String searchQuery, String path) {
    // Iterate through all files and directories in the current directory
        for (FileSystemElement file : dir.getChildren()) {
            // If the current file matches the search query, printing its path
            if (file.getName().equals(searchQuery)) {
                System.out.println("Found: " + path);
                return true;
            }

            // If the current element is a directory, recursively searching in it
            if (file instanceof Directory) {
                // Update the path for the recursive call
                String newPath = path + "/" + file.getName();
                // Cast the file to Directory and recursively search within it
                SearchForFiles((Directory) file, searchQuery, newPath);
            }
        }
        return false;
    }


    /**
     * Printing the directory tree by using our current path to iterate and finally printing the files and directories of our current directory
     * regarding to their types using '*' for directories and also printing ' ' as in the example output.
     * @param currentDirectory
     * @param currentPath
     */
    public void PrintDirectoryTree(Directory currentDirectory , String currentPath){ 
        System.out.println("Path to current directory from root : ");
        String[] names = currentPath.split("/");
        for(int i=1;i<names.length;i++){
            for(int j=1;j<i;j++)
                System.out.print(" ");
            if(names[i].equals(currentDirectory.getName())){
                System.out.print("* " + names[i] +  "/");
                System.out.println(" (Current Directory)");
                for(FileSystemElement f : currentDirectory.getChildren()){
                   for(int j=1;j<i;j++)
                        System.out.print(" ");
                            if(f instanceof File)
                                System.out.println(f.getName());
                            else if(f instanceof Directory)
                                System.out.println("* " + f.getName());
                }
                return;
            }
            else{
                System.out.println("* " + names[i] +  "/");
            }
        }
    }
    
    /**
     * Sorting the contents of the current directory by first taking the size and creating a temp array to assign the children to here for sorting.
     * Then, in a loop it sorts the array which includes the children using bubble sort by using before method of timestamp
     * finally it prints the sorted array.
     * @param currentDirectory
     */
    public void SortContents(Directory currentDirectory) {
    int size = currentDirectory.getChildren().size();
    FileSystemElement[] temp = new FileSystemElement[size];
    FileSystemElement nt;
    for (int i = 0; i < size; i++) {
        temp[i] = currentDirectory.getChildren().get(i);
    }

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if(temp[j].getDateCreated().before(temp[i].getDateCreated())){
                nt = temp[i];
                temp[i] = temp[j];
                temp[j] = nt;
            }
        }
    }

        for (int k = 0; k < size; k++) {
            if (temp[k] instanceof File)
                System.out.println(temp[k].name + "  " + temp[k].getDateCreated());
            else if (temp[k] instanceof Directory)
            System.out.println("* " + temp[k].name + "  " + temp[k].getDateCreated());
        }
    }   
}