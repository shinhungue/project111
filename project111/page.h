class Page {
    public:
        Page(int x, int y, int w, int h, int id, char con);
    private:
        int x, y; // position of the page on the board
        int width, height; // width and height of the page
        int id; // unique id for each page
        char content;
        char* page;
};

Page::Page(int x, int y, int w, int h, int id, char con){

    this->width = w;
    this->height = h;
    this->content = con;
    this->x= x;
    this->y= y;
    this->id = id;

    page = new char[w*h];

    for(int i=y;i<y+h;i++){
        for(int j=x;j<x+w;j++){
            page[i*w+j] = con;
        }
    }

}

