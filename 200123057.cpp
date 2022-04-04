#include<bits/stdc++.h>
using namespace std;

typedef struct bucket{
    int local_depth = 0;
    vector<int> array;
} bucket;

int key_to_index(int key, int global_depth) {
    int ans = 0;
    for (int i = 0; i < global_depth; ++i) {
        ans += (key % 2) * pow(2, i);
        key = key / 2;
    }
    return ans;
}

class ext_hashing {
    public:
    vector<bucket*> directory;
    int g_depth;
    int b_capacity;
    int no_of_buckets = 0;
    int counter = 0;
    unordered_map <int, vector<int>> counter_vs_bucketidx;

    ext_hashing(int global_depth, int bucket_capacity) {
        g_depth = global_depth;
        b_capacity = bucket_capacity;
        for (int i = 0; i < pow(2, global_depth); ++i) {
            bucket* temp = new bucket;
            //Initially global_depth = local_depth
            temp->local_depth = global_depth;
            directory.push_back(temp);
            ++no_of_buckets;

            if (counter_vs_bucketidx.find(counter) != counter_vs_bucketidx.end()) {
                counter_vs_bucketidx[counter].push_back(i);
            }

            else {
                vector<int> temp_vector;
                temp_vector.push_back(i);
                counter_vs_bucketidx[counter] = temp_vector;
            }
        }
    }

    void insert(int key) {
        int index = key_to_index(key, g_depth);

        if ((directory[index]->array).size() == b_capacity) {
            int l_depth = directory[index]->local_depth;

            while (l_depth != g_depth) {
                //creating new bucket
                bucket* temp = new bucket();
                temp->local_depth = l_depth + 1;
                ++counter;
                ++no_of_buckets;

                //b_idx --> index considering local depth as l_depth + 1
                //l_idx --> index considering local depth as l_depth
                //k_idx --> index for key considering local depth as l_depth
                int b_idx, l_idx;
                int k_idx = key_to_index(key, l_depth);

                for (int i = 0; i < directory.size(); ++i) {
                    b_idx = key_to_index(i, l_depth) + pow(2, l_depth + 1);
                    l_idx = key_to_index(i, l_depth);

                    if (counter_vs_bucketidx.find(counter) != counter_vs_bucketidx.end()) {
                        counter_vs_bucketidx[counter].push_back(b_idx); 
                    }

                    else {
                        vector<int> temp_vector;
                        temp_vector.push_back(i);
                        counter_vs_bucketidx[counter] = temp_vector;
                    }

                    if (k_idx == l_idx) {
                        //assigning new bucket to b_idx in order to create more buckets and increase local depth
                        if (key_to_index(i, l_depth + 1) == b_idx) {
                            directory[b_idx] = temp;
                        }

                        //previously shared bucket is assigned to l_idx with increased in local depth
                        else {
                            directory[l_idx]->local_depth = l_depth + 1;
                        }
                    }
                }

                //local depth increased by one
                l_depth = l_depth + 1;

                //rearranging keys
                vector<int> idx_delete;
                for (int i = 0; i < (directory[k_idx]->array).size(); ++i) {
                    int idx = key_to_index((directory[k_idx]->array)[i], l_depth);
                    if (k_idx != idx) {
                        (directory[idx]->array).push_back((directory[k_idx]->array)[i]);
                        idx_delete.push_back(i);
                    }
                }

                for (int i = 0; i < idx_delete.size(); ++i) {
                    vector<int>::iterator it = (directory[k_idx]->array).begin() + idx_delete[i];
                    (directory[k_idx]->array).erase(it);
                }

                if ((directory[index]->array).size() < b_capacity) {
                    (directory[index]->array).push_back(key);
                    return;
                }
            }

            if (l_depth == g_depth) {

                while (1) {
                    //Expanding directory
                    for (int i = pow(2, g_depth); i < pow(2, g_depth + 1); ++i) {
                        directory[index]->local_depth = g_depth + 1;

                        //fully occupied slot --> asssign new bucket
                        if (key_to_index(i, g_depth) == index) {
                            //create new bucket
                            bucket* temp = new bucket;
                            temp->local_depth = g_depth + 1;
                            ++counter;
                            ++no_of_buckets;
                            
                            directory[i] = temp;
                            vector<int> temp_vector;
                            temp_vector.push_back(i);
                            counter_vs_bucketidx[counter] = temp_vector;
                        }

                        else {
                            directory[i] = directory[key_to_index(i, g_depth)];
                        }
                    }

                    //global depth increased
                    g_depth += 1;

                    //rearranging keys
                    vector<int> idx_delete;
                    for (int i = 0; i < (directory[index]->array).size(); ++i) {
                        int idx = key_to_index((((directory[index])->array)[i]), g_depth);
                        if (index != idx) {
                            (directory[idx]->array).push_back((directory[index]->array)[i]);
                            idx_delete.push_back(i);
                        }
                    }

                    for (int i = 0; i < idx_delete.size(); ++i) {
                        vector<int>::iterator it = (directory[index]->array).begin() + idx_delete[i];
                        (directory[index]->array).erase(it);
                    }

                    if ((directory[key_to_index(key, g_depth)]->array).size() < b_capacity) {
                        (directory[index]->array).push_back(key);
                        return;
                    }
                }
            }
        }

        else {
            // cout << index << "\n";
            (directory[index]->array).push_back(key);
            return;
        }
    }


    void search(int key) {
        int index = key_to_index(key, g_depth);
        for (int i = 0; i < (directory[index]->array).size(); ++i) {
            if ((directory[index]->array)[i] == key) {
                cout << "key is present\n";
                return;
            }
        }

        cout << "key is absent\n";
        return;
    }

    void delete_key(int key) {
        int index = key_to_index(key, g_depth);
        for (int i = 0; i < (directory[index]->array).size(); ++i) {
            if ((directory[index]->array)[i] == key) {
                vector<int>::iterator it = (directory[index]->array).begin() + i;
                (directory[index]->array).erase(it);
                return;
            }
        }
    }

    void display() {
        cout << "global_depth: " << g_depth << endl;
        cout << "no_of_buckets: " << no_of_buckets << "\n\n";

        for (int i = 0; i < counter + 1; ++i) {
            for (int j = 0; j < counter_vs_bucketidx[i].size(); ++j) {
                cout << "bucket: " << (counter_vs_bucketidx[i])[j] << " No. of keys: " << (directory[(counter_vs_bucketidx[i])[j]]->array).size() << " local_depth: " << directory[(counter_vs_bucketidx[i])[j]]->local_depth << endl;
                for (int k = 0; k < (directory[(counter_vs_bucketidx[i])[j]]->array).size(); ++k) {
                    cout << (directory[(counter_vs_bucketidx[i])[j]]->array)[k] << " ";
                }

                cout << "\n\n";
            }
        }

        return;
    }

    // void delete_with_merging(int key) {
    // }
};

int main() {
    int global_depth, bucket_capacity, operation, key;
    cin >> global_depth;
    cin >> bucket_capacity;

    ext_hashing* table = new ext_hashing(global_depth, bucket_capacity);

    cin >> operation;
    while (operation != 6) {
        if (operation == 2 || operation == 3|| operation == 4) {
            cin >> key;

            switch (operation){
            case 2:
                (*table).insert(key);
                break;

            case 3:
                (*table).search(key);
                break;

            case 4:
                (*table).delete_key(key);
                break;
            }
        }

        else {
            (*table).display();
        }

        cin >> operation;
    }

    return 0;
}