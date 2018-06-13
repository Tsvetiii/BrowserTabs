#ifndef TAB_MODULE_CPP
#define TAB_MODULE_CPP

#include"DoubleLinkedList.cpp"
#include <ctime>
#include <iostream>
#include <cstring>
#include <sstream>

long int unix_timestamp()
{
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}

struct TabData{
    std::string URL;
    long int timestamp;

    TabData(std::string _URL = "about:blank", long int _time = unix_timestamp()) :
        URL(_URL), timestamp(_time) {}
};

class TabModule{
private:
    DoubleLinkedList<TabData> tabs;
    DoubleLinkedListIterator<TabData> current;

public:
    friend TabModule mergeByURL(TabModule const& l1, TabModule const& l2);
    friend TabModule mergeByTime(TabModule const& l1, TabModule const& l2);
    friend void mergeSortByURL(TabModule& tabModule);
    friend void mergeSortByTime(TabModule& tabModule);
    friend void split(TabModule const& lst, TabModule& l1, TabModule& l2);

    long int getTime(DoubleLinkedListIterator<TabData> iter) const{
        return iter.get().timestamp;
    }

    std::string getURL(DoubleLinkedListIterator<TabData> iter) const{
        return iter.get().URL;
    }

    TabModule(){
        TabData tabdata;
        tabs.insertBegin(tabdata);
        current = tabs.begin();
    }

    void backTab(){
        if(current == tabs.begin())
            return;
        current = current.prev();
    }

    void forwardTab(){
        if(current == tabs.end())
            return;
        current = current.next();
    }

    void removeTab(){
        if(tabs.begin() == tabs.end()){
            goTab("about:blank");
            return;
        }

        if(current == tabs.end()){
            tabs.deleteAt(current);
            current = current.prev();
            return;
        }

        tabs.deleteAt(current);
        current = current.next();
    }

    void goTab(std::string url){
        TabData tab(url);
        current.get() = tab;
    }

    void insertTab(std::string url){
        TabData tab(url);
        tabs.insertAfter(url, current);
        current = current.next();
    }

    void printTabs(){
        for(DoubleLinkedListIterator<TabData> iter = tabs.begin(); iter; ++iter){
            if(iter == current) std::cout<<">";
            std::cout<<getURL(iter)<<" "<<getTime(iter)<<std::endl;
        }
    }

    void run(){
        std::string input;
        do{
			getline(std::cin, input);
			doCommand(input);
        }while(input != "EXIT");
    }

    void doCommand(std::string input){
        std::istringstream commands(input);
        std::string command, url;
        char c;
        bool hasURL = false;
        while(commands.get(c)){
            if(c == ' ') hasURL = true;
            else{
                if(!hasURL)
                    command += c;
                else
                    url += c;
            }
        }
        if(!hasURL)
            findCommand(command);
        else findURLCommand(command, url);
    }

    void findCommand(std::string command){
        if(command == "BACK")
            backTab();
        else if(command == "FORWARD")
            forwardTab();
        else if(command == "REMOVE")
            removeTab();
        else if(command == "PRINT")
            printTabs();
    }

    void findURLCommand(std::string command, std::string url){
        if(command == "GO")
            goTab(url);
        else if(command == "INSERT")
            insertTab(url);
        //else if(command == "SORT"){
        //    if(url == "URL")
        //        mergeSortByURL(*this);
        //    if(url == "TIME")
        //        mergeSortByTime(*this);
        //}
    }
};


TabModule mergeByURL(TabModule const& l1, TabModule const& l2){
    TabModule newTabModule;
    DoubleLinkedListIterator<TabData> it1 = l1.tabs.begin(), it2 = l2.tabs.begin();
    while(it1 && it2){
        if(l1.getURL(it1).compare(l2.getURL(it2)) < 0){
            newTabModule.tabs.insertEnd(*it1++);
        } else if (l1.getURL(it1).compare(l2.getURL(it2)) > 0){
            newTabModule.tabs.insertEnd(*it2++);
        } else {
            if(l1.getTime(it1) <= l2.getTime(it2))
                newTabModule.tabs.insertEnd(*it1++);
            else newTabModule.tabs.insertEnd(*it2++);
        }
    }

    while(it1){
        newTabModule.tabs.insertEnd(*it1++);
    }

    while(it2){
        newTabModule.tabs.insertEnd(*it2++);
    }

    newTabModule.current = newTabModule.tabs.begin();
    return newTabModule;
}


TabModule mergeByTime(TabModule const& l1, TabModule const& l2){
    TabModule newTabModule;
    DoubleLinkedListIterator<TabData> it1 = l1.tabs.begin(), it2 = l2.tabs.begin();
    while(it1 && it2){
        if(l1.getTime(it1) < l2.getTime(it2)){
            newTabModule.tabs.insertEnd(*it1++);
        } else if (l1.getTime(it1) > l2.getTime(it2)){
            newTabModule.tabs.insertEnd(*it2++);
        } else {
            if(l1.getURL(it1).compare(l2.getURL(it2)) < 0)
                newTabModule.tabs.insertEnd(*it1++);
            else newTabModule.tabs.insertEnd(*it2++);
        }
    }

    while(it1){
        newTabModule.tabs.insertEnd(*it1++);
    }

    while(it2){
        newTabModule.tabs.insertEnd(*it2++);
    }

    newTabModule.current = newTabModule.tabs.begin();
    return newTabModule;
}

void split(TabModule const& lst, TabModule& l1, TabModule& l2){
    TabModule *now = &l1, *later = &l2;
    for(DoubleLinkedListIterator<TabData> iter = lst.tabs.begin(); iter; ++iter){
        now->tabs.insertEnd(*iter);
        std::swap(now, later);
    }
}


void mergeSortByURL(TabModule& tabModule){
    TabModule l1, l2;
    split(tabModule, l1, l2);
    mergeSortByURL(l1);
    mergeSortByURL(l2);
    tabModule = mergeByURL(l1, l2);
}


void mergeSortByTime(TabModule& tabModule){
    TabModule l1, l2;
    split(tabModule, l1, l2);
    mergeSortByTime(l1);
    mergeSortByTime(l2);
    tabModule = mergeByTime(l1, l2);
}

#endif // TAB_MODULE_CPP
