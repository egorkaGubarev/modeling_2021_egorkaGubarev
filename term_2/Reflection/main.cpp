#include <fstream>
#include <cmath>
#include <vector>

typedef unsigned char uchar;
typedef float my_type;

template <typename type>
class Segment{
public:
    Segment(): k_defined_{false} {};

    Segment(const type x1, const type y1, const type x2, const type y2)
    {
        if (x1 < x2){
            x1_ = x1;
            x2_ = x2;
            y1_ = y1;
            y2_ = y2;
        }else{
            if (x1 > x2){
                x1_ = x2;
                x2_ = x1;
                y1_ = y2;
                y2_ = y1;
            }else{
                if (x1 == x2){
                    x1_ = x1;
                    x2_ = x2;
                    y1_ = std::min(y1, y2);
                    y2_ = std::max(y1, y2);
                }
            }
        }
        const type delta_x = x2_ - x1_;
        if (delta_x != 0){
            k_defined_ = true;
            const type delta_y = y2_ - y1_;
            k_ = delta_y / delta_x;
            b_ = y1_ - k_ * x1_;
        }else{
            k_defined_ = false;
        }
    }

    Segment(const Segment<type>& source): x1_{source.x1_}, x2_{source.x2_}, y1_{source.y1_}, y2_{source.y2_},
    k_{source.k_}, b_{source.b_}, k_defined_{source.k_defined_} {}

    ~Segment() = default;

    Segment<type>& operator=(const Segment<type>& source)
    {
        x1_ = source.x1_;
        x2_ = source.x2_;
        y1_ = source.y1_;
        y2_ = source.y2_;
        k_ = source.k_;
        b_ = source.b_;
        k_defined_ = source.k_defined_;
        return *this;
    }

    [[nodiscard]] type get_x1() const
    {
        return x1_;
    }

    [[nodiscard]] type get_x2() const
    {
        return x2_;
    }

    [[nodiscard]] type get_k() const
    {
        return k_;
    }

    [[nodiscard]] type get_b() const
    {
        return b_;
    }

    [[nodiscard]] bool is_k_defined() const
    {
        return k_defined_;
    }

    friend std::ostream& operator << (std::ostream& out, const Segment<my_type>& segment)
    {
        out << segment.x1_ << ' ' << segment.y1_ << ' ' << segment.x2_ << ' ' << segment.y2_;
        return out;
    }

private:
    type x1_, x2_, y1_, y2_, k_, b_;
    bool k_defined_;
};

template <typename type>
class Ray{
public:
    Ray(const type x, const type y, const type angle): x_{x}, y_{y}, angle_{angle}, epsilon_{0.0000001}
    {
        if (angle_ > M_PI / 2 - epsilon_ && angle_ < M_PI / 2 + epsilon_){
            k_defined_ = false;
            vertical_direction_ = "up";
            horizontal_direction_ = "none";
        }
        if (angle_ > - M_PI / 2 - epsilon_ && angle_ < - M_PI / 2 + epsilon_){
            k_defined_ = false;
            vertical_direction_ = "down";
            horizontal_direction_ = "none";
        }
        if ((angle_ < - M_PI / 2 - epsilon_ || angle_ > - M_PI / 2 + epsilon_) &&
            (angle_ < M_PI / 2 - epsilon_ || angle_ > M_PI / 2 + epsilon_))
        {
            k_defined_ = true;
            k_ = tan(angle_);
            b_ = y_ - k_ * x_;
            if (angle_ > 0 && angle < M_PI){
                vertical_direction_ = "up";
            }
            if (angle_ < 0 && angle > -M_PI){
                vertical_direction_ = "down";
            }
            if (angle_ < M_PI / 2 && angle_ > - M_PI / 2){
                horizontal_direction_ = "right";
            }
            if (angle_ > - M_PI && angle_ < - M_PI / 2 || angle_ > M_PI / 2 && angle_ < M_PI){
                horizontal_direction_ = "left";
            }
            if (angle_ == 0){
                horizontal_direction_ = "right";
                vertical_direction_ = "none";
            }
            if ((angle_ > M_PI - epsilon_ && angle_ < M_PI + epsilon_) ||
                (angle_ > - M_PI - epsilon_ && angle_ < - M_PI + epsilon_))
            {
                horizontal_direction_ = "left";
                vertical_direction_ = "none";
            }
        }
    }

    [[nodiscard]] type get_x() const
    {
        return x_;
    }

    [[nodiscard]] type get_y() const
    {
        return y_;
    }

    [[nodiscard]] type get_k() const
    {
        return k_;
    }

    [[nodiscard]] type get_b() const
    {
        return b_;
    }

    [[nodiscard]] bool is_k_defined() const
    {
        return k_defined_;
    }

    [[nodiscard]] const std::string& get_horizontal_direction() const
    {
        return horizontal_direction_;
    }

    [[nodiscard]] const std::string& get_vertical_direction() const
    {
        return vertical_direction_;
    }

    void reflect(const type x, const type y, const Segment<type>& segment)
    {
        x_ = x;
        y_ = y;
        const bool segment_not_vertical = segment.is_k_defined();
        if (segment_not_vertical){
            const type k2 = segment.get_k();
            if (k_defined_){
                const std::string horizontal_before = horizontal_direction_;
                angle_ = 2 * std::atan(k2) - std::atan(k_);
                make_general_reflection();
                if (horizontal_before == "left"){
                    reverse();
                }
            }else{
                const std::string vertical_before = vertical_direction_;
                angle_ = 2 * std::atan(k2) - M_PI;
                make_general_reflection();
                if (vertical_before == "down"){
                    reverse();
                }
            }
        }else{
            angle_ = -angle_;
            k_ = - k_;
            b_ = y_ - k_ * x_;
            if (horizontal_direction_ == "left"){
                horizontal_direction_ = "right";
            }else{
                if (horizontal_direction_ == "right"){
                    horizontal_direction_ = "left";
                }
            }
        }
    }

private:
    void normalize_angle()
    {
        const type factor = angle_ / (2 * M_PI);
        if (factor > 0){
            const uchar rotations = std::floor(factor);
            const type delta = rotations * 2 * M_PI;
            angle_ = angle_ - delta;
        }else{
            if (factor < 0){
                const char rotations = std::ceil(factor);
                const type delta = rotations * 2 * M_PI;
                angle_ = angle_ - delta;
            }
        }
        if(angle_ < - M_PI){
            angle_ += 2 * M_PI;
        }else{
            if(angle_ > M_PI){
                angle_ -= 2 * M_PI;
            }
        }
    }

    void make_general_reflection()
    {
        normalize_angle();
        if (angle_ != M_PI / 2 && angle_ != - M_PI / 2){
            k_ = std::tan(angle_);
            b_ = y_ - k_ * x_;
            if (angle_ > -M_PI / 2 && angle_ < M_PI / 2){
                horizontal_direction_ = "right";
            }
            else{
                horizontal_direction_ = "left";
            }
            if (angle_ > 0 && angle_ < M_PI){
                vertical_direction_ = "up";
            }
            if (angle_ > - M_PI && angle_ < 0){
                vertical_direction_ = "down";
            }
        }
        else{
            k_defined_ = false;
            if (angle_ == M_PI / 2){
                vertical_direction_ = "up";
            }
            else{
                vertical_direction_ = "down";
            }
        }
    }

    void reverse()
    {
        if (horizontal_direction_ == "right"){
            horizontal_direction_ = "left";
        }else{
            horizontal_direction_ = "right";
        }
        if (vertical_direction_ == "up"){
            vertical_direction_ = "down";
        }else{
            vertical_direction_ = "up";
        }
    }

    const type epsilon_;
    type x_, y_, angle_, k_, b_;
    bool k_defined_;
    std::string horizontal_direction_, vertical_direction_;
};

template <typename type>
class Vessel{
public:
    explicit Vessel(std::vector<Segment<type>>&&  segments, const Ray<type>& ray): segments_{std::move(segments)},
    ray_{ray}, epsilon_{0.00001} {}

    void evaluate(const std::string& path, const uchar amount)
    {
        std::ofstream out;
        out.open(path);
        for(uchar i = 0; i < amount; ++ i){
            find_reflection(out);
        }
        out.close();
    }

private:
    static type get_distance(const type x1, const type x2, const type y1, const type y2)
    {
        const type delta_x = x2 - x1;
        const type delta_y = y2 - y1;
        const type distance = std::sqrt(std::pow(delta_x, 2) + std::pow(delta_y, 2));
        return distance;
    }

    static void find_collision_point(const type x, const type y, const type distance, const Segment<type>& segment,
                     type& x_ref, type& y_ref, type& distance_ref, Segment<type>& segment_ref, bool& reflected)
    {
        if (!reflected || distance < distance_ref){
            x_ref = x;
            y_ref = y;
            distance_ref = distance;
            segment_ref = segment;
            reflected = true;
        }
    }

    std::ofstream& find_reflection(std::ofstream& out)
    {
        type x_ref, y_ref, distance_ref;
        Segment<type> segment_tmp;
        Segment<type>& segment_ref = segment_tmp;
        bool reflected = false;
        const bool ray_k_is_defined = ray_.is_k_defined();
        if (ray_k_is_defined){
            const type x1 = ray_.get_x();
            const type y1 = ray_.get_y();
            out << x1 << ' ' << y1 << ' ';
            const type k1 = ray_.get_k();
            const type b1 = ray_.get_b();
            const std::string& horizontal_direction = ray_.get_horizontal_direction();
            const std::string& vertical_direction = ray_.get_vertical_direction();
            for (const Segment<type>& segment: segments_){
                if (segment.is_k_defined()){
                    const type x1_segment = segment.get_x1();
                    const type x2_segment = segment.get_x2();
                    const type k2 = segment.get_k();
                    const type b2 = segment.get_b();
                    const type delta_k = k1 - k2;
                    if (delta_k != 0){
                        const type y = (k1 * b2 - k2 * b1) / delta_k;
                        const type x = (b2 - b1) / delta_k;
                        if (x > x1_segment){
                            if (x < x2_segment){
                                const type delta_y = y - y1;
                                const type delta_x = x - x1;
                                const type distance = get_distance(x, x1, y, y1);
                                if (distance > epsilon_){
                                    if (delta_y > 0){
                                        if (vertical_direction == "up"){
                                            find_collision_point(x, y, distance, segment, x_ref, y_ref, distance_ref,
                                                                 segment_ref, reflected);
                                        }
                                    }else{
                                        if (delta_y < 0){
                                            if (vertical_direction == "down"){
                                                find_collision_point(x, y, distance, segment, x_ref, y_ref, distance_ref,
                                                                     segment_ref, reflected);
                                            }
                                        }
                                        else{
                                            if (delta_x > 0){
                                                if (horizontal_direction == "right"){
                                                    find_collision_point(x, y, distance, segment, x_ref, y_ref,
                                                                         distance_ref, segment_ref, reflected);
                                                }
                                            }
                                            else{
                                                if (delta_x < 0){
                                                    if (horizontal_direction == "left"){
                                                        find_collision_point(x, y, distance, segment, x_ref, y_ref,
                                                                             distance_ref, segment_ref, reflected);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }else{
                    const type x = segment.get_x1();
                    const type y = k1 * x + b1;
                    const type delta_x = x - x1;
                    const type distance = get_distance(x, x1, y, y1);
                    if (distance > epsilon_){
                        if(delta_x > 0 && horizontal_direction == "right"){
                            find_collision_point(x, y, distance, segment, x_ref, y_ref, distance_ref, segment_ref,
                                                 reflected);
                        }
                        else{
                            if(delta_x < 0 && horizontal_direction == "left") {
                                find_collision_point(x, y, distance, segment, x_ref, y_ref, distance_ref, segment_ref,
                                                     reflected);
                            }
                        }
                    }
                }
            }
        }else{
            const type x1 = ray_.get_x();
            const type y1 = ray_.get_y();
            const std::string& vertical_direction = ray_.get_vertical_direction();
            for (const Segment<type> &segment: segments_) {
                if (segment.is_k_defined()) {
                    const type x1_segment = segment.get_x1();
                    const type x2_segment = segment.get_x2();
                    const type k2 = segment.get_k();
                    const type b2 = segment.get_b();
                    const type y = k2 * x1 + b2;
                    if (x1 > x1_segment) {
                        if (x1 < x2_segment) {
                            const type delta_y = y - y1;
                            if (delta_y > epsilon_){
                                if (delta_y > 0) {
                                    if (vertical_direction == "up") {
                                        find_collision_point(x1, y, delta_y, segment, x_ref, y_ref, distance_ref,
                                                             segment_ref, reflected);
                                    }
                                } else {
                                    if (vertical_direction == "down") {
                                        find_collision_point(x1, y, delta_y, segment, x_ref, y_ref, distance_ref,
                                                             segment_ref, reflected);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        out << x_ref << ' ' << y_ref << '\n';
        ray_.reflect(x_ref, y_ref, segment_ref);
        return out;
    }

    std::vector<Segment<type>> segments_;
    Ray<type> ray_;
    const type epsilon_;
};

int main()
{
    const uchar amount = 100;
    const std::string path = "result.txt";
    const std::string path_config = "config.txt";
    const Segment<my_type> left(0, 0, 0, 2);
    const Segment<my_type> right(2, 0, 2, 2);
    const Segment<my_type> up(0, 2, 2, 2);
    const Segment<my_type> down(0, 0, 2, 0);
    const Segment<my_type> s1(0.1, 1.2, 0.7, 1.3);
    const Segment<my_type> s2(0.7, 1.3, 1, 1.8);
    const Segment<my_type> s3(1, 1.8, 1.3, 1.3);
    const Segment<my_type> s4(1.3, 1.3, 1.9, 1.2);
    const Segment<my_type> s5(1.9, 1.2, 1.5, 0.7);
    const Segment<my_type> s6(1.5, 0.7, 1.6, 0.1);
    const Segment<my_type> s7(1.6, 0.1, 1, 0.3);
    const Segment<my_type> s8(1, 0.3, 0.4, 0.1);
    const Segment<my_type> s9(0.4, 0.1, 0.5, 0.7);
    const Segment<my_type> s10(0.5, 0.7, 0.1, 1.2);
    std::vector<Segment<my_type>> segments{left, right, up, down, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10};
    std::ofstream config;
    config.open(path_config);
    for (const Segment<my_type>& segment: segments){
        config << segment << '\n';
    }
    config.close();
    Ray<my_type> ray(1, 1, M_PI / 6);
    Vessel<my_type> vessel(std::move(segments), ray);
    vessel.evaluate(path, amount);
    return 0;
}
