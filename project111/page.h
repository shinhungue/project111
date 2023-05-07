#include <vector>
#include <algorithm>
class Page {
    public:
          Page(int x, int y, int width, int height, int id, int content) :
               x(x), y(y), width(width), height(height), id(id), content(content) {}

          Page(){};
        int get_id() const { return id; }
        int get_x() const { return x; }
        int get_y() const { return y; }
        int get_width() const { return width; }
        int get_height() const { return height; }
        char get_content() const { return content; }

        void set_x(int x) { this->x = x; }
        void set_y(int y) { this->y = y; }
        void set_content(char content) { this->content = content; }
        static bool compare(const Page& a,const Page& b){
          return a.id<b.id;
        }
       static void sort_pages(std::vector<Page>& p);


    private:
        int x, y; // position of the page on the board
        int width, height; // width and height of the page
        int id; // unique id for each page
        char content;

};

void Page::sort_pages(std::vector<Page> &p){
  std::sort(p.begin(),p.end(),compare);
}

