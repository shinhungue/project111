#include <fstream>

#include "page.h"
#include <vector>
#include <algorithm>

using std::endl;
using std::ofstream;
bool is_print_board_enabled =true;

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

    private:
        int num_jobs, width, height;
        ofstream& output;
        char* board;
        std::vector<Page> pages;
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
  pages.push_back(page);
  update_board(page, true);
  if(is_print_board_enabled == true){
  print_board();
  }
}

void Board::delete_page(int id) {
  std::vector<Page> d_vec;
  int idx = -1;
  for (int i = pages.size() - 1; i >= 0; i--) {
      if (pages[i].get_id() == id) {
          idx = i;
          break;
        }
    }          // id에 따라 pages에서의 index확보..

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
    delete_page(pages[iidx].get_id());
  }
}
// 지우는 작업 + 붙이는 작업
Page dt_page(pages[idx].get_x(),pages[idx].get_y(),pages[idx].get_width(),pages[idx].get_height(), pages[idx].get_id(),' ');
  update_board(dt_page, true);
  print_board();
}





void Board::modify_content(int id, char content) {


}
void Board::modify_position(int id, int x, int y) {


}
