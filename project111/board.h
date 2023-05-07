#include <fstream>

#include "page.h"
#include <vector>
#include <algorithm>

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
  void removes_page(int id);

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
  std::vector<Page> dPage;
  dPage.resize(pages.size());
  copy(pages.begin(),pages.end(),dPage.begin());  //pages 복사본 dPage

  int idx = -1;
  for (int i = dPage.size() - 1; i >= 0; i--) {
      if (dPage[i].get_id() == id) {
          idx = i;
          break;
        }
    }          // id 해당 page 찾기(idx) dPage 에서


  std::vector<Page> tracking_page;
  std::vector<std::pair<int,Page>> index_v;
  for(int z=0;z<dPage.size();z++){
    index_v.push_back({z,dPage[z]});
  }

  std::vector<int> V1;
  for(int i=dPage[idx].get_x();i<dPage[idx].get_x()+dPage[idx].get_width();i++){
    V1.push_back(i);
  }
  std::vector<int> V6;
  for(int i=dPage[idx].get_y();i<dPage[idx].get_y()+dPage[idx].get_height();i++){
    V6.push_back(i);
  }

  for(int c=idx+1;c<dPage.size();c++){
    std::vector <int> V2;
    std::vector <int> V4;
    std::vector <int> V3(V1.size()+V2.size());
    std::vector <int> V5(V6.size()+V4.size());

    for(int i=dPage[c].get_x();i<dPage[c].get_x()+dPage[c].get_width();i++){
    V2.push_back(i);
  }
  for(int i=dPage[idx].get_y();i<dPage[idx].get_y()+dPage[idx].get_height();i++){
    V4.push_back(i);
  }
    auto iterx = set_intersection(V1.begin(),V1.end(),V2.begin(),V2.end(),V3.begin());
    auto itery = set_intersection(V6.begin(),V6.end(),V4.begin(),V4.end(),V5.begin());
    V3.erase(iterx,V3.end());
    sort(V3.begin(),V3.end());
    V5.erase(itery,V5.end());
    sort(V5.begin(),V5.end());  // V3, V5 는 각각 x,y 좌표값 교집합 (곂치는 부분)

    bool inter = false;
  if(dPage[idx].get_x()<dPage[c].get_x()<dPage[idx].get_x()+dPage[idx].get_width() || dPage[idx].get_x()<dPage[c].get_x()+dPage[c].get_width()<dPage[idx].get_x()+dPage[idx].get_width()  || dPage[idx].get_y()<dPage[c].get_y()<dPage[idx].get_y()+dPage[idx].get_height() || dPage[idx].get_y()<dPage[c].get_y()+dPage[c].get_height()<dPage[idx].get_y()+dPage[idx].get_height())
{
  inter = true;
}
    // delete 대상 page 와 곂치는 부분 있는지 확인
  bool above = true;
    for(int r=idx+1;r<c;r++)
    {
      if((dPage[c].get_x()<V3.front()<dPage[c].get_x()+dPage[c].get_width()) && (
        dPage[c].get_x()<V3.back()<dPage[c].get_x()+dPage[c].get_width()) &&(
        dPage[c].get_y()<V5.front()<dPage[c].get_y()+dPage[c].get_height()) &&(
        dPage[c].get_y()<V5.back()<dPage[c].get_y()+dPage[c].get_height()))
      {
        above = false;
      }

        }
// above 인지 확인

    if(inter==true && above==true){
      tracking_page.push_back(dPage[c]);
    }
  }
  // above page 있는지 확인

  if(tracking_page.size()>0){
    Page::sort_pages(tracking_page);
    for(int a=0;a<tracking_page.size();a++){
      delete_page(tracking_page[a].get_id());
    }
  }

    // above page=0 이면 delete
    Page deleted_page(dPage[idx].get_x(),dPage[idx].get_y(), dPage[idx].get_width(), dPage[idx].get_height(), dPage[idx].get_id(), ' ');
    update_board(deleted_page, true);
    // 지워주고

    int id_index= -1;
    for(int b=0;b<index_v.size();b++){
    if( index_v[b].second.get_id()== dPage[idx].get_id()){
      id_index =index_v[b].first;
      break;
    }
  }

     for(int p=0;p<id_index;p++){
      Page back_page(dPage[p].get_x(),dPage[p].get_y(),dPage[p].get_width(),dPage[p].get_height(),dPage[p].get_id(),dPage[p].get_content());
      update_board(back_page,true);
    }
    // 밑에 깔려있는 얘들 복구시켜주고
    print_board();
    dPage.erase(dPage.begin()+idx);


}
void Board::removes_page(int id) {
    int idx = -1;
    for (int i = pages.size() - 1; i >= 0; i--) {
        if (pages[i].get_id() == id) {
            idx = i;
            break;
        }
    }


    for(int i=pages.size()-1;i>=idx;i--){
      Page remove_page(pages[i].get_x(),pages[i].get_y(),pages[i].get_width(),pages[i].get_height(),pages[i].get_id(),' ');
      update_board(remove_page,true);
      for(int j=1;j<=i;j++){
        Page new_page(pages[j-1].get_x(),pages[j-1].get_y(),pages[j-1].get_width(),pages[j-1].get_height(),pages[j-1].get_id(),pages[j-1].get_content());
      update_board(new_page, true);
      }
    }

}

void Board::modify_content(int id, char content) {
  int idx = -1;
  for(int i = pages.size()-1;i>=0;i--){
    if(pages[i].get_id()==id){
      idx = i;
      break;
    }
  }
  for(int j=pages.size()-1;j>=idx;j--){
    removes_page(pages[j].get_id());
  }
    print_board();

  pages[idx].set_content(content);
  for(int k=idx;k<pages.size();k++){
    Page new_modify_page(pages[k].get_x(),pages[k].get_y(),pages[k].get_width(),pages[k].get_height(),pages[k].get_id(),pages[k].get_content());
    update_board(new_modify_page,true);
  }
  print_board();
  }

void Board::modify_position(int id, int x, int y) {
  int idx = -1;
  for(int i = pages.size()-1;i>=0;i--){
    if(pages[i].get_id()==id){
      idx = i;
      break;
    }
  }
}
