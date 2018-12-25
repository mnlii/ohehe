//
// Created by tahara on 18-12-12.
//

#ifndef BOOKSTORE_BLL_H
#define BOOKSTORE_BLL_H

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
using std::vector;

#define BLOCK 512

/*
 * DT is the data type of your data;
 * KT is the key type of your data;
 *   You must reload the operator '==' for the DT.
 *   When first use database, use setFunction(void*, void*) to appoint your own function to your key.
 */
template <typename KT, typename DT>
class BLL {
private:

    // header is the data header for your file.
    char path[50]{};
    char lock[50]{};
    FILE* fp;
    /*
     * compare function for keys;
     * qual(KT& a, KT& b)   returns true when a == b;
     * cmp(KT& a, KT& b)    returns true when a < b;
     * ngt(KT& a, KT& b)    returns true when a <= b;
     */
    bool (*cmp)(const KT&, const KT&);
    bool (*equal)(const KT&, const KT&);
    bool ngt(const KT&, const KT&);
    /*
     * meta is the header struct for database.
     * head: indicate the head of the list.
     * slot: the space database has used.
     * data_cnt:  the number of data store in database.
     * block_cnt: the number of disk block used by database.
     */
    struct meta{
        off_t head;
        off_t slot;
        size_t data_cnt;
        size_t block_cnt;
    }meta_h;

    struct node{
        KT key[BLOCK];
        DT data[BLOCK];
        size_t top;
        off_t nxt;
        off_t bef;
        off_t loc;
        /*
        const node& operator=(const node& a){
            memcpy(key, a.key, sizeof(key));
            memcpy(data, a.data, sizeof(data));
            top = a.top;
            bef = a.bef;
            loc = a.loc;
            return a;
        }*/
        node(){
            memset(key, 0, sizeof(key));
            memset(data, 0, sizeof(data));
            top = static_cast<size_t>(nxt = bef = loc = 0);
        }
    };


    /*
     * judge the file "filename" whether is exist.
     * if "filename" existed, return true, else return false;
     */
    bool exist_file(const char* filename);

    /*
     * because this is a sample system.
     * we use a single lock to guarantee the concurrency of our database;
    */

    /*
     * use dbopen() to connect to your database file.
     */
    bool dbopen();

    /*
     * disconnect to your database file.
     */
    void dbclose();

    /*
     * use map to read data from disk;
     */
    size_t  map(void* data, off_t offt, size_t size);

    /*
     * use unmap to write data to disk;
     *
     */
    void unmap(void* data, off_t offt, size_t size);

    /*
     * move data from para "from" to para "to";
     */
    void movedata(off_t from, off_t to);

    /*
     * after delete data from database, check whether the block has to merge.
     * if (now.top + (bef.top | nxt.top) < BLOCK / 2) we will merge two block.
     */
    void merge(node& nd);

public:
    bool indexFlg;
    vector<DT> dlist;
    vector<off_t> old_t;
    vector<off_t> new_t;

    off_t lst_query;
    BLL() = delete;
    BLL(const char* fpath);

    void setFunction(bool (*kcm)(const KT&, const KT&), bool (*keq)(const KT&, const KT&));

    bool query(const KT& key, const DT& data);
    vector<DT> query(const KT& key);
    vector<DT> getall();

    off_t insert(const KT& key, const DT& data);
    DT getdata(off_t offt);
    bool update(const KT& key, const DT& data);
    bool removedata(const KT& key, const DT& data);
    int getDTN();
};



template <typename KT, typename DT>
BLL<KT, DT>::BLL(const char* fpath){
    indexFlg = false;
    strcpy(path, fpath);
    strcpy(lock, fpath);
    int len = static_cast<int>(strlen(lock));
    lock[len++] = '.';
    lock[len++] = 'l';
    lock[len++] = 'k';
    lock[len] = 0;
    if(!exist_file(fpath)){
        meta_h.block_cnt = 0;
        meta_h.data_cnt = 0;
        meta_h.head = 0;
        meta_h.slot = sizeof(meta);
        fp = fopen(path, "wb");
        unmap(&meta_h, 0, sizeof(meta));
        fclose(fp);
    }

}


template <typename KT, typename DT>
bool BLL<KT, DT>::exist_file(const char *filename) {
    FILE* fptr = fopen(filename, "r");
    if(fptr){
        fclose(fptr);
        return true;
    }
    return false;
}

template<typename KT, typename DT>
void BLL<KT, DT>::unmap(void *data, off_t offt, size_t size){
    fseek(fp, offt, SEEK_SET);
    fwrite(data, size, 1, fp);
}

template<typename KT, typename DT>
size_t BLL<KT, DT>::map(void *data, off_t offt, size_t size) {
    fseek(fp, offt, SEEK_SET);
    size_t res = fread(data, size, 1, fp);
    return res;
}
template<typename KT, typename DT>
void BLL<KT, DT>::setFunction(bool (*kcm)(const KT &, const KT &), bool (*keq)(const KT &, const KT &)) {
    cmp = kcm;
    equal = keq;
}

template<typename KT, typename DT>
bool BLL<KT, DT>::dbopen() {
    // get the lock of database.
    if(exist_file(lock))
        return false;

    FILE* fptr = fopen(lock, "w");
    fclose(fptr);

    fp = fopen(path, "rb+");
    // get the header data from file.
    map(&meta_h, 0, sizeof(meta));
    return true;
}

template<typename KT, typename DT>
void BLL<KT, DT>::dbclose() {
    // write the header data back to database;
    unmap(&meta_h, 0, sizeof(meta));

    // release the lock of database.
    remove((const char*)lock);
    fclose(fp);
}

template<typename KT, typename DT>
bool BLL<KT, DT>::query(const KT &key, const DT &data) {
    node now;
    int loc;
    if(meta_h.head == 0){
        return false;
    }


    map(&now, meta_h.head, sizeof(node));
    while(true) {
        if (ngt(key, now.key[now.top - 1])) {
            loc = static_cast<int>(std::lower_bound(now.key, now.key + now.top, key, cmp) - now.key);

            while(loc < now.top){
                if(now.data[loc] == data) return true;
                loc++;
                if(loc == now.top){
                    if(now.nxt == 0) return false;
                    map(&now, now.nxt, sizeof(node));
                }
            }
        }
        else {
            if (now.nxt == 0) {
                dbclose();
                return false;
            }
            map(&now, now.nxt, sizeof(node));
        }
    }
    return false;
}

template<typename KT, typename DT>
vector<DT> BLL<KT, DT>::query(const KT &key) {
    vector<DT> res;
    int loc;
    dbopen();
    node now;
    if(meta_h.head == 0){
        dbclose();
        return res;
    }
    map(&now, meta_h.head, sizeof(node));

    while(true){
        if(ngt(key, now.key[now.top - 1])){
            loc = static_cast<int>(std::lower_bound(now.key, now.key + now.top, key, cmp) - now.key);

            while(equal(now.key[loc], key)){
                res.push_back(now.data[loc]);
                lst_query = now.loc + (size_t) &(((node*)0)->data) + sizeof(DT) * loc;

                loc++;
                if(loc == now.top){
                    loc = 0;
                    if(now.nxt == 0)
                        break;
                    map(&now, now.nxt, sizeof(node));
                }
            }
            break;
        }
        else{
            if(now.nxt == 0) break;
            map(&now, now.nxt, sizeof(node));
        }
    }
    dbclose();
    return res;
}


template<typename KT, typename DT>
off_t BLL<KT, DT>::insert(const KT &key, const DT &data) {
    node now;
    off_t res;
    size_t loc, i;
    dbopen();
    if(indexFlg) {
        dlist.clear();
        new_t.clear();
        old_t.clear();
    }

    if(meta_h.head == 0){
        memset(&now, 0, sizeof(node));
        memcpy(&now.key[0], &key, sizeof(KT));
        memcpy(&now.data[0], &data, sizeof(DT));
        now.top = 1;
        meta_h.head = now.loc = meta_h.slot;

        unmap(&now, now.loc, sizeof(node));
        meta_h.slot += sizeof(node);

        res = now.loc + (size_t) &(((node*)0)->data);

        meta_h.block_cnt++;
        meta_h.data_cnt++;

        dbclose();
        return res;
    }

    map(&now, meta_h.head, sizeof(node));
    while(true){
        if(now.nxt == 0 || ngt(key, now.key[now.top - 1])) {
            loc = std::lower_bound(now.key, now.key + now.top, key, cmp) - now.key;
            i = now.top;
            while(i > loc){
                memcpy(now.key + i, now.key + i - 1, sizeof(KT));
                memcpy(now.data + i, now.data + i - 1, sizeof(DT));
                if(indexFlg) {
                    dlist.push_back(now.data[i]);
                    old_t.push_back(now.loc + (size_t) &(((node *) 0)->data) + sizeof(DT) * (i - 1));
                    new_t.push_back(now.loc + (size_t) &(((node *) 0)->data) + sizeof(DT) * i);
                }
                i--;
            }
            memcpy(now.key + loc, &key, sizeof(KT));
            memcpy(now.data + loc, &data, sizeof(DT));
            now.top++;
            res = now.loc + (size_t) &(((node*)0)->data) + sizeof(DT) * loc;


            if(now.top == BLOCK){
                node nxt, tmp;
                nxt.loc = meta_h.slot;
                meta_h.slot += sizeof(node);

                for(i = 0; i < now.top / 2; i++){
                    memcpy(nxt.key + i, now.key + i + now.top / 2, sizeof(KT));
                    memcpy(nxt.data + i, now.data + i + now.top / 2, sizeof(DT));
                    if(indexFlg) {
                        dlist.push_back(now.data[i + now.top / 2]);
                        old_t.push_back(now.loc + (size_t) &(((node *) 0)->data) + sizeof(DT) * (i + now.top / 2));
                        new_t.push_back(nxt.loc + (size_t) &(((node *) 0)->data) + sizeof(DT) * i);
                    }
                }

                now.top = nxt.top = BLOCK / 2;
                if(now.nxt != 0) {
                    map(&tmp, now.nxt, sizeof(node));
                    tmp.bef = nxt.loc;
                    unmap(&tmp, tmp.loc, sizeof(node));
                }
                nxt.nxt = now.nxt;
                nxt.bef = now.loc;
                now.nxt = nxt.loc;

                unmap(&nxt, nxt.loc, sizeof(node));

                meta_h.block_cnt++;
            }
            unmap(&now, now.loc, sizeof(node));
            break;
        }
        else{
            map(&now, now.nxt, sizeof(node));
        }
    }
    meta_h.data_cnt++;
    dbclose();
    return res;
}

template<typename KT, typename DT>
bool BLL<KT, DT>::removedata(const KT &key, const DT& data) {
    dbopen();
    node now, tmp;
    unsigned int loc, i;
    map(&now, meta_h.head, sizeof(node));

    while(true){
        if(now.top != 0 && ngt(key, now.key[now.top - 1])){
            loc = static_cast<unsigned int>(std::lower_bound(now.key, now.key + now.top, key, cmp) - now.key);
            for(; loc < now.top; loc++){
                if(now.data[loc] == data) break;
                if(loc == now.top - 1){
                    loc = 0;
                    if(now.nxt == 0) return false;
                    map(&now, now.nxt, sizeof(node));
                }
            }
            for(; loc < now.top - 1; loc++){
                memcpy(now.key + loc, now.key + loc + 1, sizeof(KT));
                memcpy(now.data + loc, now.data + loc + 1, sizeof(DT));
            }
            now.top--;
            meta_h.data_cnt--;

            // delete data from disk and reformat the data on disk;
            if(meta_h.data_cnt == 0){
                meta_h.slot = meta_h.slot - sizeof(node);
                meta_h.block_cnt = 0;
                meta_h.head = 0;
            }
            else if(meta_h.block_cnt != 1){
                merge(now);
            }
            break;
        }
        else{
            if(now.nxt == 0) return false;
            map(&now, now.nxt, sizeof(node));
        }
    }
    unmap(&now, now.loc, sizeof(node));

    dbclose();
    return true;
}


template<typename KT, typename DT>
bool BLL<KT, DT>::ngt(const KT & a, const KT & b) {
    return cmp(a, b) || equal(a, b);
}

template<typename KT, typename DT>
void BLL<KT, DT>::movedata(off_t from, off_t to) {
    node dt, tmp;
    map(&dt, from, sizeof(node));
    if(dt.bef != 0){
        map(&tmp, dt.bef, sizeof(node));
        tmp.nxt = to;
        unmap(&tmp, tmp.loc, sizeof(node));

    }
    if(dt.nxt != 0){
        map(&tmp, dt.nxt, sizeof(node));
        tmp.bef = to;
        unmap(&tmp, tmp.loc, sizeof(node));
    }
    unmap(&dt, to, sizeof(node));
}

template<typename KT, typename DT>
void BLL<KT, DT>::merge(BLL::node &nd) {
    node tmp;
    int i;
    if(nd.bef != 0){
        map(&tmp, nd.bef, sizeof(node));
        if(tmp.top + nd.top <= BLOCK / 2){
            for(i = 0; i < tmp.top; i++){
                memcpy(nd.key + nd.top + i, tmp.key + i, sizeof(KT));
                memcpy(nd.data + nd.top + i, tmp.data + i, sizeof(DT));
            }
            nd.top = nd.top + tmp.top;
            nd.bef = tmp.bef;

            // re-format the struct of db-file;
            movedata(meta_h.slot - sizeof(node), tmp.loc);
            meta_h.slot -= sizeof(node);

            if(nd.bef != 0){
                map(&tmp, nd.bef, sizeof(node));
                tmp.nxt = nd.loc;
                unmap(&tmp, tmp.loc, sizeof(node));
            }
            return;
        }
    }

    if(nd.nxt != 0){
        map(&tmp, nd.nxt, sizeof(node));
        if(tmp.top + nd.top <= BLOCK / 2){
            for(i = 0; i < tmp.top; i++){
                memcpy(nd.key + nd.top + i, tmp.key + i, sizeof(KT));
                memcpy(nd.data + nd.top + i, tmp.data + i, sizeof(DT));
            }
            nd.top = nd.top + tmp.top;
            nd.nxt = tmp.nxt;

            // re-format the struct of db-file;
            movedata(meta_h.slot - sizeof(node), tmp.loc);
            meta_h.slot -= sizeof(node);

            if(nd.nxt != 0){
                map(&tmp, nd.nxt, sizeof(node));
                tmp.bef = nd.loc;
                unmap(&tmp, tmp.loc, sizeof(node));
            }
            return;
        }
    }
}

template<typename KT, typename DT>
bool BLL<KT, DT>::update(const KT &key, const DT &data) {
    dbopen();
    node now;
    int loc;

    if(meta_h.head == 0){
        dbclose();
        return false;
    }
    map(&now, meta_h.head, sizeof(node));
    while(true){
        if(ngt(key, now.key[now.top - 1])){
            loc = static_cast<int>(std::lower_bound(now.key, now.key + now.top, key, cmp) - now.key);
            memcpy(now.data + loc, &data, sizeof(DT));
            lst_query = now.loc + (size_t) &(((node*)0)->data) + sizeof(DT) * loc;
            unmap(&now, now.loc, sizeof(node));
            break;
        }
        else{
            if(now.nxt == 0) {
                dbclose();
                return false;
            }
            map(&now, now.nxt, sizeof(node));
        }
    }
    dbclose();
    return false;
}

template<typename KT, typename DT>
DT BLL<KT, DT>::getdata(off_t offt) {
    DT res;
    dbopen();
    map(&res, offt,sizeof(DT));
    dbclose();
    return res;
}


template<typename KT, typename DT>
vector<DT> BLL<KT, DT>::getall() {
    dbopen();
    vector<DT> res;
    node now;
    int i;
    if(meta_h.head == 0){
        dbclose();
        return res;
    }
    map(&now, meta_h.head, sizeof(node));
    while(true){
        for(i = 0; i < now.top; i++){
            res.push_back(now.data[i]);
        }
        if(now.nxt == 0){
            break;
        }
        else{
            map(&now, now.nxt, sizeof(node));
        }
    }
    dbclose();
    return res;
}

template<typename KT, typename DT>
int BLL<KT, DT>::getDTN() {
    dbopen();
    size_t res = meta_h.data_cnt;
    dbclose();
    return static_cast<int>(res + 1);
}

#endif //BOOKSTORE_BLL_H
