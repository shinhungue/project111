#include <fstream>

#include "page.h"
#include <vector>
#include <algorithm>

using std::endl;
using std::ofstream;


class Board {
    public:
        Board(int num_jobs, int width, int height, ofstream& output_stream);
        ~Board();

        void print_board();
        void print_job(int job_idx, char job_type, int id);

        //job functions
        void insert_page(int x, int y, int width, int height, int id, int content);
        void delete_page(int id);
        void modify_content(int id, char content);
        void modify_position(int id, int x, int y);
        void remove_page(int id);

    private:
        int num_jobs, width, height;
        ofstream& output;
        char* board;
        std::vector<Page> pages;
        std::vector<Page> T_pages;
        std::vector<int> id_call;
        int remove_num=-1;
void update_board(const Page &page, bool add) {
    char symbol = add ? page.get_content() : '.';
    for (int i = page.get_y(); i < page.get_y() + page.get_height(); i++) {
      for (int j = page.get_x(); j < page.get_x() + page.get_width(); j++) {
        board[i * width + j] = symbol;
      }
    }
  }
};

Board::Board(int num_jobs, int width, int height, ofstream& output_stream): output(output_stream) {
    this->width = width;
    this->height = height;
    this->num_jobs = num_jobs;

    board = new char[width*height];

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h*width + w] = ' ';
        }
    }

}

Board::~Board() {
    delete board;

}


void Board::print_board() {
    int h, w;
    for (w = 0; w < width+2; w++) output << "- ";
    output << endl;

    for (h = 0; h < height; h++) {
        output << "| ";
        for (w = 0; w < width; w++) {
            output << board[h*width + w] << " ";
        }
        output << "| " << endl;
    }

    for (w = 0; w < width+2; w++) output << "- ";
    output << endl;
}

void Board::print_job(int job_idx, char job_type, int id) {
    output << ">> (" << job_idx <<") ";
    switch(job_type) {

        case 'i':
            output << "Insert ";
            break;
        case 'd':
            output << "Delete ";
            break;
        case 'm':
            output << "Modify ";
            break;
    }

    output << id << endl;
}


void Board::insert_page(int x, int y, int width, int height, int id, int content) {
  Page page(x,y,width,height,id,content);
  T_pages.push_back(page);
  pages.push_back(page);
  update_board(page, true);
  print_board();

}

void Board::remove_page(int id) {

  std::vector<Page> d_vec;
  int idx = -1;
  for (int i = pages.size() - 1; i >= 0; i--) {
      if (pages[i].get_id() == id) {
          idx = i;
          break;
        }
    }          // id에 따라 pages에서의 index확보..
if(remove_num==-1){
  remove_num = idx;
}


for(int i=idx+1;i<pages.size();i++){
  bool overlap = false;
  bool over = true;
  bool xoverlap = pages[idx].get_x()<(pages[i].get_x()+pages[i].get_width()) &&
                  (pages[idx].get_x()+pages[idx].get_width()) > pages[i].get_x();
  bool yoverlap = pages[idx].get_y()<(pages[i].get_y()+pages[i].get_height()) &&
                  (pages[idx].get_y()+pages[idx].get_height()) > pages[i].get_y();
  if(xoverlap==true&&yoverlap==true){
    overlap = true;
    int mx = std::max(pages[idx].get_x(), pages[i].get_x());
    int mix = std::min(pages[idx].get_x()+pages[idx].get_width(),
    pages[i].get_x()+pages[i].get_width());
    int my = std::max(pages[idx].get_y(), pages[i].get_y());
    int miy = std::min(pages[idx].get_y()+pages[idx].get_height(),
    pages[i].get_y()+pages[i].get_height());
    Page testP(mx,my,mix-mx,miy-my,-1,0);
    for(int j =idx+1;j<i;j++){
      bool iscontain = (testP.get_x()>= pages[j].get_x())&&
                       (testP.get_y()>=pages[j].get_y()) &&
        (testP.get_width()+testP.get_x()<=pages[j].get_x()+pages[j].get_width())&&
      (testP.get_y()+testP.get_height() <= pages[j].get_y()+pages[j].get_height());
      if(iscontain==true){
        over = false;
        break;
      }
    }
  }
if(overlap== true&& over==true){
  d_vec.push_back(pages[i]);
}
}

if(d_vec.size()>0){

  std::sort(d_vec.begin(),d_vec.end(),compare);
  for(int i=0;i<d_vec.size();i++){
    int iidx = -1;
    for(int j=pages.size()-1;j>=0;j--){
      if(pages[j].get_id()==d_vec[i].get_id()){
        iidx = j;
        break;
      }
    }
    if(pages[iidx].get_content()!=' '){
    remove_page(pages[iidx].get_id());
    }
  }
}
// 지우는 작업 + 붙이는 작업
bool overlaped = false;
    for (int i = pages[remove_num].get_y(); i < pages[remove_num].get_y() + pages[remove_num].get_height(); i++) {
      for (int j = pages[remove_num].get_x(); j < pages[remove_num].get_x() + pages[remove_num].get_width(); j++) {
        if(board[i*width+j]!= pages[remove_num].get_content())
          overlaped = true;
        break;
      }
    }
if(overlaped ==true){
pages[idx].set_content(' ');
update_board(pages[idx], true);
for(int b=0;b<pages.size();b++){
  if(pages[b].get_content()!=' '){
    update_board(pages[b], true);
  }
}

  id_call.push_back(pages[idx].get_id());
  print_board();
}
if(remove_num == idx){
  pages[idx].set_content(' ');
update_board(pages[idx], true);
for(int b=0;b<pages.size();b++){
  if(pages[b].get_content()!=' '){
    update_board(pages[b], true);
  }
}

  id_call.push_back(pages[idx].get_id());
  print_board();
}

}


void Board::delete_page(int id){
  remove_page(id);
  for(int i=id_call.size()-2;i>=0;i--){
    int idx = -1;
    for(int j=0;j<pages.size();j++){
      if(pages[j].get_id()==id_call[i]){
        idx = j;
        break;
      }
    }
    // pages 복구 작업 ( 당사page제외)
    if(idx != -1){
    pages[idx].set_content(T_pages[idx].get_content());
    update_board(pages[idx],true);
    print_board();
  }
  }
  /// T_pages, pages에서 완전 삭제 + id_call erase
  for(int a=0;a<T_pages.size();a++){
    if(T_pages[a].get_id()==id_call[id_call.size()-1]){
      T_pages.erase(T_pages.begin()+a);
      break;
    }
  }
  for(int a=0;a<pages.size();a++){
    if(pages[a].get_id()==id_call[id_call.size()-1]){
      pages.erase(pages.begin()+a);
      break;
    }
  }
  id_call.erase(id_call.begin(),id_call.end());
  remove_num=-1;
}

void Board::modify_content(int id, char content) {
  remove_page(id);
  int idx = -1;
  for(int i=0;i<T_pages.size();i++){
    if(T_pages[i].get_id()==id_call[id_call.size()-1]){
      idx = i;
      break;
    }
  }
  T_pages[idx].set_content(content);
  pages[idx].set_content(content);
  update_board(pages[idx],true);
  print_board();

  for(int a=id_call.size()-2;a>=0;a--){
    int iidx = -1;
    for(int j=0;j<T_pages.size();j++){
      if(T_pages[j].get_id()==id_call[a]){
        iidx = j;
        break;
      }
    }
    if(iidx != -1){
    pages[iidx].set_content(T_pages[iidx].get_content());
    update_board(pages[iidx],true);
    print_board();
  }
  }
  id_call.erase(id_call.begin(),id_call.end());
  remove_num=-1;
}
void Board::modify_position(int id, int x, int y) {
   remove_page(id);
    int idx = -1;
  for(int i=0;i<T_pages.size();i++){
    if(T_pages[i].get_id()== id_call[id_call.size()-1]){
      idx = i;
      break;
    }
  }

  for(int i=0;i<idx;i++){

  bool xoverlap = T_pages[idx].get_x()<(T_pages[i].get_x()+T_pages[i].get_width()) &&
                  (T_pages[idx].get_x()+T_pages[idx].get_width()) > T_pages[i].get_x();
  bool yoverlap = T_pages[idx].get_y()<(T_pages[i].get_y()+T_pages[i].get_height()) &&
                  (T_pages[idx].get_y()+T_pages[idx].get_height()) > T_pages[i].get_y();
    if(xoverlap==true && yoverlap==true){
      int xma = std::max(T_pages[idx].get_x(),T_pages[i].get_x());
      int xmi = std::min(T_pages[idx].get_x()+T_pages[idx].get_width(), T_pages[i].get_x()+ T_pages[i].get_width());
      int yma = std::max(T_pages[idx].get_y(),T_pages[i].get_y());
      int ymi = std::min(T_pages[idx].get_y()+T_pages[idx].get_height(), T_pages[i].get_y()+ T_pages[i].get_height());
      Page overlap_Page(xma,yma,xmi-xma,ymi-yma,-1,T_pages[i].get_content());
      update_board(overlap_Page,true);
    }
  }

  T_pages[idx].set_x(x);
  T_pages[idx].set_y(y);
  pages[idx].set_content(T_pages[idx].get_content());
  pages[idx].set_x(x);
  pages[idx].set_y(y);

  update_board(pages[idx],true);
  print_board();

    for(int a=id_call.size()-2;a>=0;a--){
    int iidx = -1;
    for(int j=0;j<T_pages.size();j++){
      if(T_pages[j].get_id()==id_call[a]){
        iidx = j;
        break;
      }
    }
    if(iidx != -1){
    pages[iidx].set_content(T_pages[iidx].get_content());
    update_board(pages[iidx],true);
    print_board();
  }
  }

  id_call.erase(id_call.begin(),id_call.end());
  remove_num = -1;
}
