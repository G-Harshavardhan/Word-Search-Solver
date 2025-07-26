#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>

using namespace std;

class TrieNode
{
    public:
        unordered_map<char, TrieNode*> children;
        bool isEndOfWord;

        TrieNode()
        {
            isEndOfWord = false;
        }
};

class Trie
{
    public:
        TrieNode* root;

        Trie()
        {
            root = new TrieNode();
        }

        void insert(const string& word)
        {
            TrieNode* node = root;
            for (char c : word)
            {
                if (!node->children.count(c)) node->children[c] = new TrieNode();
                node = node->children[c];
            }
            node->isEndOfWord = true;
        }
};

void backtrack(vector<vector<char>>& board, TrieNode* node, int i, int j, string path, unordered_set<string>& result, map<string, vector<pair<int, int>>>& wordLocations, vector<pair<int, int>>& currentPath)
{
    if (node->isEndOfWord)
    {
        result.insert(path);
        node->isEndOfWord = false;
        wordLocations[path] = currentPath;
    }
    if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size()) return;
    char temp = board[i][j];
    if (node->children.find(temp) == node->children.end()) return;
    node = node->children[temp];
    board[i][j] = '#';
    currentPath.push_back({i, j});
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (const auto& dir : directions)
    {
        backtrack(board, node, i + dir.first, j + dir.second, path + temp, result, wordLocations, currentPath);
    }
    board[i][j] = temp;
    currentPath.pop_back();
}

unordered_set<string> findWords(vector<vector<char>>& board, vector<string>& words, map<string, vector<pair<int, int>>>& wordLocations) {
    Trie trie;
    for (const string& word : words)
    {
        trie.insert(word);
    }
    unordered_set<string> result;
    for (int i = 0; i < board.size(); ++i)
    {
        for (int j = 0; j < board[0].size(); ++j)
        {
            vector<pair<int, int>> currentPath;
            backtrack(board, trie.root, i, j, "", result, wordLocations, currentPath);
        }
    }
    return result;
}

void printBoard(const vector<vector<char>>& board)
{
    cout << "\nWord search grid :\n";
    for (const auto& row : board)
    {
        for (char cell : row)
        {
            cout << cell << ' ';
        }
        cout << endl;
    }
}

void printWordLocations(const map<string, vector<pair<int, int>>>& wordLocations)
{
    cout << "\nWord Locations :\n";
    for (const auto& entry : wordLocations)
    {
        cout << entry.first << ": ";
        for (const auto& pos : entry.second)
        {
            cout << "(" << pos.first << ", " << pos.second << ") ";
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    int rows, cols;
    cout << "\nEnter the number of rows in the grid    : ";
    cin >> rows;
    cout << "Enter the number of columns in the grid : ";
    cin >> cols;
    vector<vector<char>> board(rows, vector<char>(cols));
    cout << "\nEnter the grid row by row (space-separated) :" << endl;
    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            cin >> board[i][j];
        }
    }
    printBoard(board);
    cout << "\nEnter words to search (space-separated) : ";
    string input;
    cin.ignore();
    getline(cin, input);  
    vector<string> words;
    size_t pos = 0;
    while ((pos = input.find(' ')) != string::npos)
    {
        words.push_back(input.substr(0, pos));
        input.erase(0, pos + 1);
    }
    words.push_back(input);
    unordered_set<string> foundWords;
    map<string, vector<pair<int, int>>> wordLocations;
    foundWords = findWords(board, words, wordLocations);
    cout << "\nFound words : ";
    if (foundWords.empty())
    {
        cout << "No words found.\n" << endl;
        return 0;
    }
    else
    {
        for (const string& word : foundWords)
        {
            cout << word << " ";
        }
        cout << endl;
    }
    printWordLocations(wordLocations);
    return 0;
}
