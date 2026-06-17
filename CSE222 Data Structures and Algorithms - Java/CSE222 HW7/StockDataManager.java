import java.io.BufferedReader;
import java.io.File ;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;
import java.util.Scanner ;
import javax.swing.*;

public class StockDataManager {
    private AVLTree AVL;

    public StockDataManager(){
        this.AVL = new AVLTree();
    }

    /**
     * Main method to try the AVLTree and plot the graph.
     * @param args
     */
    public static void main(String[] args){

        StockDataManager sdm = new StockDataManager();

        System.out.println("Graph is being plotted...");
        SwingUtilities.invokeLater(() -> {
            String plotType = "line"; // Change to "scatter" for scatter plot
            GUIVisualization frame = new GUIVisualization(plotType,1000,"ADD"); // Create a new instance of GUIVisualization
            frame.setVisible(true); // Make the frame visible        // NOTE : this "     " part should be changed for the operation choice of the graph. 
        });                                                            // ADD/REMOVE/UPDATE/SEARCH/MIXED
        
        if (args.length != 1) {
            System.out.println("Usage: java Main <input_file>");
            sdm.ScriptToFileMixed(50);
            sdm.InputFormat("GStock.txt");
            return;
        }
        
        //sdm.ScriptToFile(100);      // to write to the file only 1 operation 100 times.

        String inputFile = args[0]; // if file name is entered by user in command line 'java DataStockManager' 'filename'

        try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
            String line;
            while ((line = br.readLine()) != null) {
                processCommand(line, sdm);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        //sdm.InputFormat(inputFile); // trying the operations in the file which was the entered by user in the command line.        

    }

    private static void processCommand(String line, StockDataManager manager) {
        String[] tokens = line.split(" ");
        String command = tokens[0];

        switch (command) {
            case "ADD":
                manager.addOrUpdateStock(tokens[1], Double.parseDouble(tokens[2]), Long.parseLong(tokens[3]), Long.parseLong(tokens[4]));
                break;
            case "REMOVE":
                manager.removeStock(tokens[1]);
                break;
            case "SEARCH":
                Stock stock = manager.searchStock(tokens[1]);
                if (stock != null) {
                    System.out.println(stock);
                } else {
                    System.out.println("Stock not found: " + tokens[1]);
                }
                break;
            case "UPDATE":
                //manager.updateStock(tokens[1], Double.parseDouble(tokens[3]), Long.parseLong(tokens[4]), Long.parseLong(tokens[5]));
                break;
            default:
                System.out.println("Unknown command: " + command);
                break;
        }
    }

    /*
     * Method for formatting the input.
     * First the fileRead scanner is created then first string which is the operation type is read and according to the operation
     * for example if it is 'ADD' all the 4 values are read and a stock is created also for update too with symbol to be updated.
     * For both update and remove only one string which is the symbol is read and after that related methods are called.  
     */
    public void InputFormat(String fileName){
        try{
            File file = new File(fileName);
            Scanner fileRead = new Scanner(file);

            while(fileRead.hasNextLine()){
                String operation = fileRead.next();
                String symbol;
                switch(operation){
                    case "ADD" :
                        symbol = fileRead.next();
                        Double price = fileRead.nextDouble() ;
                        Long volume = fileRead.nextLong();
                        Long marketcap = fileRead.nextLong();
                        Stock newStock = new Stock(symbol,price,volume,marketcap) ;
                        addOrUpdateStock(symbol,price,volume,marketcap);
                        break;
                    case "REMOVE" :
                        symbol = fileRead.next();
                        //System.out.println("Symbol to be deleted is " + symbol);
                        removeStock(symbol);
                        break;
                    case "SEARCH" :
                        symbol = fileRead.next();
                        searchStock(symbol);
                        break;
                    case "UPDATE" :
                        symbol = fileRead.next();
                        //System.out.print("Symbol to be updated is " + symbol + " --- ");
                        newStock = new Stock(fileRead.next(),fileRead.nextDouble(),fileRead.nextLong(),fileRead.nextLong()) ;
                        updateStock(symbol,newStock);
                        break;
                    default :
                        System.out.println("Exception in the file! Other than Add,search,remove or update is encountered.");
                        break;
                }
                
            }
        }
        catch(Exception ex){
            //System.out.println("An exception has occured while formatting the input.Please check the txt file or the method!");
        }
        AVL.PreOrderTraversal();
    }

    /**
     * adding the stock item using add method of AVLTree if it already exists then updating its value.
     * @param newItem
     */
    public void addOrUpdateStock(String symbol, double price, long volume, long marketCap) {
        Stock existingStock = AVL.searchNode(symbol);
        if (existingStock != null) {
            existingStock.setPrice(price);
            existingStock.setVolume(volume);
            existingStock.setMarketCap(marketCap);
        } else {
            Stock newStock = new Stock(symbol, price, volume, marketCap);
            AVL.addNode(newStock);
        }
    }

    /**
     * removing the stock item using remove method of AVLTree. 
     * @param symbol
     */
    public void removeStock(String symbol){
        AVL.removeNode(symbol);
    }

    /**
     * searching the stock item using search method of AVLTree.
     * @param symbol
     * @return TRUE OR FALSE
     */
    public Stock searchStock(String symbol){
        return AVL.searchNode(symbol);
    }

    /**
     * updating the stock item using update method of AVLTree.
     * @param symbol
     * @param Item
     */
    public void updateStock(String symbol , Stock Item){
        Stock stock = AVL.searchNode(symbol);
        if (stock != null) {
            stock.setPrice(Item.getPrice());
            stock.setVolume(Item.getVolume());
            stock.setMarketCap(Item.getMarketCap());
        }
    }

    /**
     * Script that writes only the entered operation to file for the given size. 
     * @param size
     */
    public void ScriptToFile(int size){
        try (FileWriter writer = new FileWriter("OneOperationTestAVL.txt");
            Scanner scanner = new Scanner(System.in)) {
            System.out.print("Enter operation (ADD, REMOVE, UPDATE, SEARCH): ");
            String operation = scanner.next();
            Random random = new Random();
            double max = 999.99;
            double min = 0.0;
            long maxLong = 999999999;
            long minLong = 9999;

            switch (operation) {
                case "ADD":
                    for (int i = 0; i < size; i++) {
                        double price = min + (max - min) * random.nextDouble();
                        long volume = minLong + (long)((maxLong - minLong) * random.nextDouble());
                        long marketCap = minLong + (long)((maxLong - minLong) * random.nextDouble());
                        writer.write(String.format("%s SYMBOL%d %.2f %d %d%n", operation, i, price, volume, marketCap));
                    }
                    break;
                case "REMOVE":
                    for (int i = 0; i < size; i++) {
                        writer.write(String.format("%s SYMBOL%d%n", operation, i));
                    }
                    break;
                case "UPDATE":
                    for (int i = 0; i < size; i++) {
                        double price = min + (max - min) * random.nextDouble();
                        long volume = minLong + (long)((maxLong - minLong) * random.nextDouble());
                        long marketCap = minLong + (long)((maxLong - minLong) * random.nextDouble());
                        writer.write(String.format("%s SYMBOL%d NEW%d %.2f %d %d%n", operation, i, i, price, volume, marketCap));
                    }
                    break;
                case "SEARCH":
                    for (int i = 0; i < size; i++) {
                        writer.write(String.format("%s SYMBOL%d%n", operation, i));
                    }
                    break;
                default:
                    System.out.println("Invalid operation.");
                    break;
            }
        } catch (Exception e) {
            
        }
    }
    /**
     * Script that writes to file as long as the given size for all the operations randomly chosen.
     * @param size
     */
    public void ScriptToFileMixed(int size){
        try (FileWriter writer = new FileWriter("MixedOperationTestAVL.txt") ) { 
            Random random = new Random();
            double max = 999.99;
            double min = 0.0;
            long maxLong = 999999999;
            long minLong = 9999;
            String operations[] = {"ADD", "REMOVE", "UPDATE", "SEARCH"} ;
            for(int i=0; i<size ; i++){
                int choice = random.nextInt(4);
                switch (operations[choice]) {
                    case "ADD":
                        double price = min + (max - min) * random.nextDouble();
                        long volume = minLong + (long)((maxLong - minLong) * random.nextDouble());
                        long marketCap = minLong + (long)((maxLong - minLong) * random.nextDouble());
                        writer.write(String.format("%s SYMBOL%d %.2f %d %d%n", operations[choice], i, price, volume, marketCap));
                        break;
                    case "REMOVE":
                          writer.write(String.format("%s SYMBOL%d%n", operations[choice], i));
                        break;
                    case "UPDATE":
                            price = min + (max - min) * random.nextDouble();
                            volume = minLong + (long)((maxLong - minLong) * random.nextDouble());
                            marketCap = minLong + (long)((maxLong - minLong) * random.nextDouble());
                            writer.write(String.format("%s SYMBOL%d NEW%d %.2f %d %d%n", operations[choice], i, i, price, volume, marketCap));
                        
                        break;
                    case "SEARCH":
                            writer.write(String.format("%s SYMBOL%d%n", operations[choice], i));
                        break;
                    default:
                        System.out.println("Invalid operation.");
                        break;
                }
        }
        } catch (Exception e) {
            
        }
    }
        
    /**
     * Performance analyse method that analyses the performance according to the operation by
     * adding/updating/removing/searching for the given size and calculates the time and returns the total time.
     * @param manager
     * @param size
     * @param operation
     * @return time
     */
    public long performPerformanceAnalysis(StockDataManager manager, int size , String operation) {
        long startTime = 0 ;
        long endTime = 0;
        Random random = new Random();
        if(operation.equals("MIXED")){
            String operations[] = {"ADD", "REMOVE", "UPDATE", "SEARCH"} ;
            int choice = random.nextInt(4);
            operation = operations[choice];
        }
        //System.out.println(operation);
        switch(operation){
            case "ADD":
                startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                    manager.addOrUpdateStock("SYM" + i, Math.random() * 100, (long) (Math.random() * 1000000), (long) (Math.random() * 1000000000));
                }
                endTime = System.nanoTime();
                break;
            case "REMOVE":
                startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                    manager.removeStock("SYM" + i);
                }
                endTime = System.nanoTime();
            break;
            case "UPDATE":
                startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                    manager.addOrUpdateStock("SYM" + i, Math.random() * 100, (long) (Math.random() * 1000000), (long) (Math.random() * 1000000000));
                }
                endTime = System.nanoTime();
            break;
            case "SEARCH":
                startTime = System.nanoTime();
                for (int i = 0; i < size; i++) {
                    manager.searchStock("SYM" + i);
                }
                endTime = System.nanoTime();
            break;
        }
        long total_time =  (endTime - startTime)  ;
        return total_time;
    }
}


