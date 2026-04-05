#include "dining_philosophers.hpp"
#include "prog_opts.hpp"

namespace lcd
{

class dining_philosophers_impl
{
    using philosopher_idx = size_t;
    class fork
    {
        std::mutex impl_;
        const size_t idx_;

    public:
        explicit fork(size_t idx) : idx_(idx) {}

        void get() { impl_.lock(); }
        void put() { impl_.unlock(); }
        size_t idx() const { return idx_; }
    };
    struct philosopher
    {
        std::binary_semaphore impl_{0};

        void begin_thinking() { impl_.acquire(); }
        void feel_hungry() { impl_.release(); }
    };
    struct action
    {
        void pick_left(philosopher_idx pidx, fork& fo)
        {
            fo.get();
            std::println(stderr, "phil:{} fork:{} act:picked-left", pidx,
                         fo.idx());
        }
        void pick_right(philosopher_idx pidx, fork& fo)
        {
            fo.get();
            std::println(stderr, "phil:{} fork:{} act:picked-right", pidx,
                         fo.idx());
        }
        void eat(philosopher_idx pidx)
        {
            std::println(stderr, "phil:{} act:eating", pidx);
        }
        void put_left(philosopher_idx pidx, fork& fo)
        {
            std::println(stderr, "phil:{} fork:{} act:put-left", pidx,
                         fo.idx());
            fo.put();
        }
        void put_right(philosopher_idx pidx, fork& fo)
        {
            std::println(stderr, "phil:{} fork:{} act:put-right", pidx,
                         fo.idx());
            fo.put();
        }
    };

    std::array<fork, 5> forks_ = {fork{0}, fork{1}, fork{2}, fork{3}, fork{4}};
    std::array<philosopher, 5> philosophers_;
    uint32_t iterations_;

public:
    dining_philosophers_impl(int argc, char** argv)
    {
        bpo::options_description opts("Options for dining philosophers");
        lcd::add_opt(opts, "info,I", "Info about available options");
        lcd::add_opt(opts, "count,C", bpo::value<uint16_t>(),
                     "Count of iterations [1 - 65535]");

        const auto vm = lcd::parse_opts(argc, argv, opts);

        iterations_ = lcd::read_opt<uint16_t>("count", vm, opts);
        if (iterations_ == 0) {
            lcd::throw_invalid_argument("'count' should be greater than 1");
        }
    }

    void run()
    {
        {
            std::jthread t1([this] { eat(1, action{}); });
            std::jthread t2([this] { eat(2, action{}); });
            std::jthread t3([this] { eat(3, action{}); });
            std::jthread t4([this] { eat(4, action{}); });
            eat(0, action{});
        }
        std::println();
    }

    void eat(philosopher_idx pidx, action act)
    {
        // There are 5 philosophers and 5 forks.
        // If each one of them picks a fork in the "same direction" e.g. the
        // left fork then they may end up in a "dead lock" situation where each
        // one of them waits the other for the right fork. To break the cycle,
        // it's enough one of them to pick first the right fork and then the
        // left one. The first philosopher picks the forks in a different order.
        for (auto _ : boost::irange(iterations_)) {
            if (pidx == 0) {
                act.pick_left(pidx, forks_.front());
                act.pick_right(pidx, forks_.back());
                act.eat(pidx);
                act.put_right(pidx, forks_.back());
                act.put_left(pidx, forks_.front());

                philosophers_.back().feel_hungry();
                philosophers_.front().begin_thinking();
            } else {
                act.pick_right(pidx, forks_[pidx - 1]);
                act.pick_left(pidx, forks_[pidx]);
                act.eat(pidx);
                act.put_left(pidx, forks_[pidx]);
                act.put_right(pidx, forks_[pidx - 1]);

                philosophers_[pidx - 1].feel_hungry();
                philosophers_[pidx].begin_thinking();
            }
        }
    }
};

////////////////////////////////////////////////////////////////////////////////

dining_philosophers::dining_philosophers(int argc, char** argv)
: impl_(std::make_unique<dining_philosophers_impl>(argc, argv))
{
}

dining_philosophers::~dining_philosophers() noexcept = default;

void dining_philosophers::run()
{
    impl_->run();
}

} // namespace lcd
