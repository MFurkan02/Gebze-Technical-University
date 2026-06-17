import java.util.LinkedList;
import java.util.Scanner;

public class TheMenu {
    public static void main(String args[]){
        TheMenu();
    }

    /**
     * Menu method for the user to manage the file system.
     */
    public static void TheMenu(){
        FileSystem fileSystem = new FileSystem();
        Directory currentDirectory = fileSystem.getRoot();
        Scanner scanner = new Scanner(System.in);
        String currentPath = "/root";

        int choice=0;
        do {
            try{
                System.out.println("===== File System Management Menu =====");
                System.out.println("1. Change directory");
                System.out.println("2. List directory contents");
                System.out.println("3. Create file/directory");
                System.out.println("4. Delete file/directory");
                System.out.println("5. Move file/directory");
                System.out.println("6. Search file/directory");
                System.out.println("7. Print directory tree");
                System.out.println("8. Sort contents by date created");
                System.out.println("9. Exit");
                System.out.print("Please select an option: ");
                choice = scanner.nextInt();
                scanner.nextLine(); // Consume newline

                switch (choice) {
                    case 1:
                        System.out.println("Current directory: " + currentPath);
                        System.out.print("Enter new directory path: ");
                        String newPath = scanner.nextLine();
                        Directory temp = currentDirectory;
                        currentDirectory=fileSystem.ChangeDirectory(fileSystem.getRoot(),newPath);
                        if(!currentDirectory.equals(temp)){
                            System.out.println("Directory changed to "+ newPath);
                            currentPath=newPath;
                        }
                        break;
                    case 2:
                        System.out.println("Listing contents of " + currentPath);
                        fileSystem.ListContents(currentDirectory,currentPath);
                        break;
                    case 3:
                        fileSystem.CreateFileorDirectory(currentDirectory,currentPath);
                        break;
                    case 4:
                        System.out.println("Current directory: " +  currentPath);
                        System.out.print("Enter name of file/directory to delete: ");
                        String fileName = scanner.nextLine();
                        LinkedList<FileSystemElement> children =new LinkedList<>();
                        children=currentDirectory.getChildren();
                        fileSystem.DeleteFileorDirectory(currentDirectory,fileName,children);
                        break;
                    case 5:
                        fileSystem.MoveFileorDirectory(currentDirectory,currentPath);
                        break;
                    case 6:
                        String path="/root";
                        System.out.print("Search query: ");
                        String searchQuery = scanner.nextLine();            
                        System.out.println("Searching from root...");
                        fileSystem.SearchForFiles(fileSystem.getRoot(),searchQuery,path);
                        break;
                    case 7:
                        fileSystem.PrintDirectoryTree(currentDirectory,currentPath);
                        break;
                    case 8:
                        System.out.println("Sorted contents of " + currentPath + " by date created : ");
                        fileSystem.SortContents(currentDirectory);
                        break;
                    case 9:
                        System.out.println("Exiting from the file management system...\nHave a nice day!");
                        break;
                    default:
                        System.out.println("Invalid option. Please enter again.");
                }
            }
            catch(Exception ex){
                System.out.println("An exception has occured! Please try again!");
            }
        } while (choice != 9);

        scanner.close();
    }
}
