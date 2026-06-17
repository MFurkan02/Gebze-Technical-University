public class AVLTree {
    private Node root;

    // Methods for insertion, deletion, searching
    private class Node {
        Stock stock;
        Node left;
        Node right;
        int height;

        public Node(Stock stock){
            this.stock = stock;
            this.left = null;
            this.right = null ;
            this.height = 1;
        }

        
    }

    private int getHeight(Node node){
        if(node==null)
            return 0;
        return node.height;
    }

    /**
     * Getting the balance of the tree which is the left subtree height - right subtree height.
     * @param node
     * @return
     */
    private int getBalance(Node node){
        if(node == null)
            return 0;
        
        // Calculate the balance factor: height of left subtree - height of right subtree
    
        return getHeight(node.left) - getHeight(node.right) ;
    }

    /**
     * getting the biggest number between two for the later usage in other methods.
     * @param x
     * @param y
     * @return
     */
    private int getBigger(int x , int y ){
        if(x > y)
            return x;
        else {
            return y;
        }
    }

    /**
     * Right rotate method which rotates the tree to the right to balance it.
     * First we take the nodes to be rotated as variables then we rotate them and update their current heights after the rotation is made.
     * @param z
     * @return
     */
    public Node rightRotate(Node z) { 

        if (z == null || z.left == null) {
            return z; // No rotation is possible if y->(z.left) or z is null.
        }

        // BEFORE THE RIGHT ROTATION

        Node y = z.left;

        Node T3 = y.right ;

        // AFTER THE RIGHT ROTATION

        z.left = T3 ;

        y.right = z ;

        // getting the new heights of both nodes.

        z.height = getBigger(getHeight(z.left) , getHeight(z.right) ) + 1; 

        y.height = getBigger(getHeight(y.left) ,getHeight(y.right)) + 1;   

        // Returning the new root 

        return y; 

    } 
   
    /**
     * Left rotate method which rotates the tree to the left to balance it.
     * First we take the nodes to be rotated as variables then we rotate them and update their current heights after the rotation is made.
     * @param z
     * @return
     */
    public Node leftRotate(Node z) {  

        if (z == null || z.right == null) {
            return z; // No rotation is possible if y->(z.left) or z is null.
        }
        // BEFORE THE LEFT ROTATION

        Node y = z.right; 

        Node T2 = y.left; 
        
        // AFTER THE LEFT ROTATION
        
        z.right  = T2;

        y.left = z;
        //  Update heights 

        y.height = getBigger(getHeight(y.left) ,getHeight(y.right)) + 1; 

        z.height = getBigger(getHeight(z.left) , getHeight(z.right) ) + 1;

        // Return new root 

        return y; 

    } 

    /**
     * to update the height of the given node.
     * @param node
     */
    private void HeightUpdate(Node node){
        if (node != null) {
            int leftHeight = (node.left == null) ? -1 : node.left.height;
            int rightHeight = (node.right == null) ? -1 : node.right.height;
            node.height = 1 + getBigger(leftHeight, rightHeight);
        }
    }


    /**
     * Method to rebalance the tree according to the current height and balance values.
     * According to the current balance value and left/right subtree values , tree is balanced using ll,lr,rr or rl rotations.
     * @param node
     * @return node
     */
    private Node rebalanceAVLTree(Node node){
        HeightUpdate(node);

        int current_balance = getBalance(node);

        if (current_balance > 1) {
            if (getBalance(node.left) >= 0) {
                return rightRotate(node);
            } else {
                node.left = leftRotate(node.left);
                return rightRotate(node);
            }
        } else if (current_balance < -1) {
            if (getBalance(node.right) <= 0) {
                return leftRotate(node);
            } else {
                node.right = rightRotate(node.right);
                return leftRotate(node);
            }
        }

        return node;
    }

    public void addNode(Stock newStock) {
        root = addNode(root, newStock);
    }
    /**
     * The add method of AVLTree.It checks the symbol of the stock to find where to add the stock.If symbol is smaller goes to left subtrees,right otherwise
     * However if it equals it updates the stock value of that symbol with the new one.
     * @param newNode node to be added.
     * @param newStock stock to be added.
     * @return updated node.
     */
    private Node addNode(Node newNode,Stock newStock){
        if(newNode == null )
            return new Node(newStock);

        int symbol_value = newStock.getSymbol().compareTo(newNode.stock.getSymbol());
        
        if(symbol_value == 0){
             newNode.stock = newStock;
             return newNode;
        }
        else if(symbol_value < 0)
            newNode.left = addNode(newNode.left,newStock);
        
        else if(symbol_value > 0)
            newNode.right = addNode(newNode.right,newStock);

        // 2. Update height of this ancestor node

        // Update height of this ancestor node
        
        newNode.height = 1 + getBigger(getHeight(newNode.left),getHeight(newNode.right)) ;

        return rebalanceAVLTree(newNode) ;
    }

    /**
     * The search method that searches the tree according to the given symbol.Goes left if the symbol value is smaller , right if bigger
     * returns the node if it 0 which means it is found.Returns null if it is not found or the node is null.
     * @param node
     * @param symbol
     * @return
     */
    private Node searchNode(Node node , String symbol){
        if(node == null) return null ;
        int symbol_value = symbol.compareTo(node.stock.getSymbol());
        if(symbol_value==0)
            return node;
        else if(symbol_value < 0){
            return searchNode(node.left, symbol);
        }
        else {
            return searchNode(node.right, symbol);
        }
    }

    /**
     * The search method of AVLTree.
     * @param symbol
     * @return
     */
    public Stock searchNode(String symbol){
        Node searchedNode = searchNode(root,symbol);
        if(searchedNode == null) return null ;
            
        return searchedNode.stock ;
    }

    public void removeNode(String symbol){
        root = removeNode(root,symbol);
    }

    /**
     * Method for removing nodes in tree.It searches the node to be removed then after it is found
     * it removes it according to its left,right or both subtrees.After the removal it updates the height of the node
     * and rebalances it.
     * @param node
     * @param symbol
     * @return
     */
    private Node removeNode(Node node, String symbol) {
        if (node == null) {
            return null;
        }
    
        int symbolValue = symbol.compareTo(node.stock.getSymbol());
    
        // If the symbol to be deleted is smaller than the current node's symbol,
        // then it goes to the left subtree
        if (symbolValue < 0) {
            node.left = removeNode(node.left, symbol);
        } 
        // If the symbol to be deleted is greater than the current node's symbol,
        // then it goes ro the right subtree
        else if (symbolValue > 0) {
            node.right = removeNode(node.right, symbol);
        } 
        // If symbol is same as the current node's symbol, then this is the node to be deleted
        else {
            // Node with only one child or no child
            if (node.left == null) {
                return node.right;
            } else if (node.right == null) {
                return node.left;
            }
    
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            Node temp = minValueNode(node.right);
    
            // Copy the inorder successor's data to this node
            node.stock = temp.stock;
    
            // Delete the inorder successor
            node.right = removeNode(node.right, temp.stock.getSymbol());
        }
    
        // Update the height of the current node
        node.height = getBigger(getHeight(node.left), getHeight(node.right)) + 1;

        // return the rebalanced tree.
        return rebalanceAVLTree(node);

    }



    /**
     * Helper method to find the node with minimum value in a given subtree for remove method. 
     * @param node
     * @return node
     */
    private Node minValueNode(Node node) {
        if (node == null) {
            return null; 
        }
        
        Node current = node;
    
        while (current.left != null) {
            current = current.left;
        }
    
        return current;
    }
    
    public void PreOrderTraversal(){
        System.out.println(" --- PRE ORDER TRAVERSAL ---");
        PreOrderTraversal(root);
    }
    /**
     * Method for PreOrderTraversal.It first prints the symbol of the root then goes to left and right subtrees respectively.
     * @param node
     */
    private void PreOrderTraversal(Node node){
        if(node==null)
            return;
        System.out.println(node.stock.printStockInfo());
        PreOrderTraversal(node.left);
        PreOrderTraversal(node.right);        
    }
    public void InOrderTraversal(){
        System.out.println(" --- IN ORDER TRAVERSAL ---");
        InOrderTraversal(root);
    }
    /**
     * Method for InOrderTraversal.It first goes to left subtrees , prints the symbol then goes to right subtree.
     * @param node
     */
    private void InOrderTraversal(Node node){
        if(node==null)
            return;
        InOrderTraversal(node.left);
        System.out.println(node.stock.printStockInfo());
        InOrderTraversal(node.right);
    }
    /**
     * Method for PostOrderTraversal.It goes left and right subtrees and prints the symbol.
     * @param node
     */
    private void PostOrderTraversal(Node node){
        if(node==null)
            return;
        PostOrderTraversal(node.left);
        PostOrderTraversal(node.right);
        System.out.println(node.stock.printStockInfo());
    }
    public void PostOrderTraversal(){
        System.out.println(" --- POST ORDER TRAVERSAL ---");
        PostOrderTraversal(root);
    }
    

}
