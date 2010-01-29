#ifndef CLIPBOARD_HXX
#define CLIPBOARD_HXX

namespace DBricks {

typedef std::vector<Shape*> ShapeList;

template<class ForwardIterT>
ShapeList* shape_list_clone(ForwardIterT begin, ForwardIterT end)
{
    ShapeList* shapes = new ShapeList;
    for (ForwardIterT iter = begin; iter != end; ++iter) {
        shapes->push_back((*iter)->clone());
    }
    return shapes;
}

inline
ShapeList* shape_list_clone(ShapeList* shapes)
{
    return shape_list_clone(shapes->begin(), shapes->end());
}

class ShapeClipboard {
public:
    bool empty() const
    {
        return m_clips.empty();
    }

    ShapeList* get_clip()
    {
        ASSERT(!empty());
        return shape_list_clone(m_clips.back());
    }


    template<class ForwardIterT>
    void put_clip(ForwardIterT begin, ForwardIterT end)
    {
        m_clips.push_back(shape_list_clone(begin, end));
    }
private:
    std::vector<ShapeList*> m_clips;
};

} // namespace DBricks


#endif // CLIPBOARD_HXX
