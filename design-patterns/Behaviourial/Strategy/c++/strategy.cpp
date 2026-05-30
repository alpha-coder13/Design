#include<iostream>
#include<vector>
#include<queue>
#include<stack>


/// implementation of strategy pattern in the below iterator pattern

class TreeNode {
    public:
        int val;
        TreeNode* left = nullptr;
        TreeNode* right= nullptr;
};

class Iterator {
    public : 
        virtual void getNext() = 0;
        virtual bool hasMore() const = 0;
        virtual int getVal() = 0;
};

class IterableCollection {
    public :
        virtual Iterator* createIterator() const = 0;
};

class TraversalStrategy {
    public:
        virtual Iterator* getIteratorForTraversal(IterableCollection*) const = 0;
};

class IterableCollectionGraph : public IterableCollection{
    protected:
        TreeNode* rootNode;
        int NodeCount;
        TraversalStrategy* strategy;
    public : 
        virtual Iterator* createIterator() const  = 0;
        void setTraversalStrategy(TraversalStrategy* strategy) {
            this->strategy = strategy;
        }
        TreeNode* getRootNode(){
            return this->rootNode;
        }
        int getNodeCount(){
            return this->NodeCount;
        }
        

};

class TreeBFSIterator : public Iterator {
    private : 
        IterableCollectionGraph* tree;
        TreeNode* curr;
        std::queue<TreeNode*> iterationQueue;
    
    public :
        TreeBFSIterator(IterableCollectionGraph* treeval , int currIndex){
            this->tree = treeval; 
            this->curr = treeval->getRootNode();
            this->iterationQueue.push(this->curr);
            // this->populateNodelist();
        }

        int getVal(){
            if(this->curr){
                return this->curr->val;
            }
        }
        
        void getNext() override{
            if(this->iterationQueue.empty())return;

            TreeNode* top = this->iterationQueue.front();
            if(top->left)this->iterationQueue.push(top->left);
            if(top->right)this->iterationQueue.push(top->right);
            this->iterationQueue.pop();
        }

        bool hasMore() const override{
            if(this->iterationQueue.empty()){
                return false;
            }
            return true;
        }

};

class TreeDFSIterator : public Iterator {
    private : 
        IterableCollectionGraph* tree;
        std::stack<TreeNode*> st;

    public :
        TreeDFSIterator(IterableCollectionGraph* treeval ){
            this->tree = treeval;
            st.push(treeval->getRootNode());
        }

        int getVal(){
             if(this->st.size() > 0){
                return this->st.top()->val;
             }
             return -1;
        }
        
        void getNext() override{
                if(this->st.size() > 0){
                    /**
                     * stack iteration logic for DFS goe shere 
                     * 
                     * 
                     */
                }
                return ;
        }

        bool hasMore() const override{
            if(this->st.size() == 0){
                return false;
            }
            return true;
        }

};

class DFSStrategy : public TraversalStrategy{
    public:
        Iterator* getIteratorForTraversal(IterableCollection* Tree)const override{
            return new TreeDFSIterator((IterableCollectionGraph*)Tree);
        }
};

class BFSStrategy : public TraversalStrategy{
    public:
        Iterator* getIteratorForTraversal(IterableCollection* Tree) const override {
            return new TreeBFSIterator((IterableCollectionGraph*)Tree,0);
        }
};


class Tree : public IterableCollectionGraph {
    public:
        Iterator* createIterator(){
            return this->strategy->getIteratorForTraversal(this);
        }
}; 

