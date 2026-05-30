#include<iostream>
#include<vector>
#include<queue>
#include<stack>

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
};

class IterableCollection {
    public :
        virtual Iterator* createIterator() const = 0;
};

class IterableCollectionGraph : public IterableCollection{
    protected:
        TreeNode* rootNode;
        int NodeCount;
    public : 
        virtual Iterator* createIterator() const  = 0; /// interviewer comment : why 3 methods , why not strategy pattern ?
        virtual Iterator* createBFSIterator() const = 0; /// what is strategy pattern ?
        virtual Iterator* createDFSIteractor() const = 0;        
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
        // std::vector<TreeNode*> NodeList;
        // void populateNodelist(){
        //     if(this->tree != nullptr){
        //         TreeNode* root= this->tree->getRootNode();
        //         std::queue<TreeNode*> Q;
        //         Q.push(root);
        //         while(!Q.empty()){
        //             TreeNode* top = Q.front();
        //             this->NodeList.push_back(top);
        //             Q.pop();
        //             if(top->left)Q.push(top->left);
        //             if(top->right)Q.push(top->right);
        //         }
        //     }
        // }
    
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

class Tree : public IterableCollectionGraph {

    public:
        Iterator* createBFSIterator() {
            return new TreeBFSIterator(this, 0);
        }

        Iterator* createDFSIteractor(){
            return new TreeDFSIterator(this);
        }

        Iterator* createIterator(){
            return nullptr;
        }
}; 

