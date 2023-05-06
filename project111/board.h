#include <fstream>

#include "page.h"
#include <vector>

using std::endl;
using std::ofstream;
bool is_print_board_enabled =true;
class Board {
public:
  Board(int num_jobs, int width, int height, ofstream &output_stream);
  ~Board();

  void print_board();
  void print_job(int job_idx, char job_type, int id);

  // job functions
  void insert_page(int x, int y, int width, int height, int id, int content);
  void delete_page(int id);
  void modify_content(int id, char content);
  void modify_position(int id, int x, int y);

private:
  int num_jobs, width, height;
  ofstream &output;
  char *board;
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

Board::Board(int num_jobs, int width, int height, ofstream &output_stream)
    : output(output_stream) {
  this->width = width;
  this->height = height;
  this->num_jobs = num_jobs;
  board = new char[width * height];

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      board[h * width + w] = ' ';
    }
  }
}

Board::~Board() { delete board; }

void Board::print_board() {
  int h, w;
  for (w = 0; w < width + 2; w++)
    output << "- ";
  output << endl;

  for (h = 0; h < height; h++) {
    output << "| ";
    for (w = 0; w < width; w++) {
      output << board[h * width + w] << " ";
    }
    output << "| " << endl;
  }

  for (w = 0; w < width + 2; w++)
    output << "- ";
  output << endl;
}

void Board::print_job(int job_idx, char job_type, int id) {
  output << ">> (" << job_idx << ") ";
  switch (job_type) {

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

void Board::insert_page(int x, int y, int width, int height, int id,
                        int content) {
  Page page(x, y, width, height, id, content);
  pages.push_back(page);
  update_board(page, true);
  if(is_print_board_enabled == true){
    print_board();
  }
}

void Board::delete_page(int id) {
    int idx = -1;
    for (int i = pages.size() - 1; i >= 0; i--) {
        if (pages[i].get_id() == id) {
            idx = i;
            break;
        }
    }


    for(int i=pages.size()-1;i>=idx;i--){
      Page delete_page(pages[i].get_x(),pages[i].get_y(),pages[i].get_width(),pages[i].get_height(),pages[i].get_id(),' ');
      update_board(delete_page,true);
      for(int j=1;j<=i;j++){
        Page new_page(pages[j-1].get_x(),pages[j-1].get_y(),pages[j-1].get_width(),pages[j-1].get_height(),pages[j-1].get_id(),pages[j-1].get_content());
      update_board(new_page, true);
      }
    if(is_print_board_enabled==true){
      print_board();
    }
    }

}

void Board::modify_content(int id, char content) {
  Page target_page;
  bool found = false;
  for(int i=0;i<width*height;i++){
    if(board[i]!='.' && pages[board[i]].get_id() == id){
      target_page = pages[board[i]];
      found = true;
      break;
    }
  }
  is_print_board_enabled = false;
  std::vector<Page> exist_pages;
  for(int i=0;i<width*height;i++){
    if(board[i]!='.'){
      exist_pages.push_back(pages[board[i]]);
      delete_page(board[i]);
    }
  }
  print_board();
  target_page.set_content(content);

  insert_page(target_page.get_x(), target_page.get_y(), target_page.get_width(), target_page.get_height(), target_page.get_id(), target_page.get_content());

  for (int i = exist_pages.size() - 1; i >= 0; i--) {
        insert_page(exist_pages[i].get_x(), exist_pages[i].get_y(), exist_pages[i].get_width(),
                    exist_pages[i].get_height(), exist_pages[i].get_id(), exist_pages[i].get_content());
    }
  print_board();
  is_print_board_enabled = true;
}
void Board::modify_position(int id, int x, int y) {}
