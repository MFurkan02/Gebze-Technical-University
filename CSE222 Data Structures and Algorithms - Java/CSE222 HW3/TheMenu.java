

import java.util.Scanner;

public class TheMenu {
    public static void main(String[] args) {

            Inventory ElecShopInventory = new Inventory() ;

            TheMenu(ElecShopInventory);
    }

    /**
     * The menu of the electronic shop inventory using a while loop till '0' is entered giving user the corresponding action regarding the entered number
     * from ther user(1-9)
     * @param ElecShopInventory
     */
    public static void TheMenu(Inventory ElecShopInventory){

        while(true){
            System.out.println("Welcome to the Electronics Inventory Management System!");
            System.out.println("Please select an option:");
            System.out.println("1. Add a new device");
            System.out.println("2. Remove a device");
            System.out.println("3. Update device details ");
            System.out.println("4. List all devices ");
            System.out.println("5. Find the cheapest device ");
            System.out.println("6. Sort devices by price");
            System.out.println("7. Calculate total inventory value ");
            System.out.println("8. Restock a device ");
            System.out.println("9. Export inventory report ");
            System.out.println("0. Exit");
            
            Scanner scanner = new Scanner(System.in);
            
        try{
            int option = scanner.nextInt();

            
            switch(option){
                case 1:
                    ElecShopInventory.addDevice();
                    break;
                case 2:
                    ElecShopInventory.removeDevice();
                    break;
                case 3:
                    ElecShopInventory.updateDevice();
                    break;
                case 4:
                    ElecShopInventory.ListAll();
                    break;
                case 5:
                    System.out.println("The cheapest device is: ");
                    Device CheapestDevice;
                    CheapestDevice=ElecShopInventory.FindCheapest();
                    System.out.println("Category: " + CheapestDevice.Category() + ", Name : " + CheapestDevice.Name() + ", Price: " + CheapestDevice.Price() + "$ ,Quantity: " + CheapestDevice.Quantity());

                    break;
                case 6:
                    ElecShopInventory.SortDevices();
                    break;
                case 7:
                    Double total_value=ElecShopInventory.CalculateTotal();
                    System.out.println("Total inventory value: $" + total_value);
                    break;
                case 8:
                    ElecShopInventory.Restock();
                    break;
                case 9:
                    ElecShopInventory.Export();
                    break;
                case 0:
                    return;
                default:
                    System.out.println("Please enter a number between 0 to 9 !");
            }
        }
        catch(Exception e){
            System.out.println("An exception has occured.Please enter appropriate values!");
        }
      }
    }
}
