#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <osal/chrono>
#include <osal/message_queue>
#include <osal/task>

using namespace std::chrono_literals;

static constexpr uint32_t kMessageIdMultiplier = 100;

template <class Duration>
void producer_task(int id,
                   const Duration delay,
                   int burstAmount,
                   osal::message_queue &msgQ)
{
    std::cout << "Starting producer_task " << id << std::endl;
    int message = id * kMessageIdMultiplier;

    while (true)
    {
        osal::this_task::sleep_for(delay);
        for (int i = 0; i < burstAmount; ++i)
        {
            std::cout << "[P" << id << "] Sending message: " << message << std::endl;
            msgQ.send(&message, sizeof(message)); 
        }
    }
}

void consumer_task(int id,
                   const std::chrono::milliseconds delay,
                   osal::message_queue &msgQ)
{
    std::cout << "Starting consumer_task" << id << std::endl;
    int message = 0;

    while (true)
    {
        msgQ.receive(&message, sizeof(message));
        std::cout << "[C" << id << "] Received message: " << message
                  << " from P" << (message / kMessageIdMultiplier) << std::endl;
        osal::this_task::sleep_for(delay);
    }
}

void osal_msg_queue_example()
{
    constexpr std::size_t stack_size = 1024;
    constexpr int priority = 2; // For this example, task priorities range from 0 - 6

    osal::message_queue msgQ("msgQ", 32, sizeof(int));

    osal::task p1(
        osal::task_attributes()
            .set_name("ProducerTask 1")
            .set_stack_size(stack_size)
            .set_priority(priority),
        producer_task<std::chrono::seconds>, 1, 1s, 3, std::ref(msgQ));

    osal::task p2(
        osal::task_attributes()
            .set_name("ProducerTask 2")
            .set_stack_size(stack_size)
            .set_priority(priority),
        producer_task<std::chrono::milliseconds>, 2, 600ms, 3, std::ref(msgQ));

    osal::task p3(
        osal::task_attributes()
            .set_name("ProducerTask 3")
            .set_stack_size(stack_size)
            .set_priority(priority),
        producer_task<std::chrono::seconds>, 3, 3s, 3, std::ref(msgQ));

    osal::task p4(
        osal::task_attributes()
            .set_name("ProducerTask 4")
            .set_stack_size(stack_size)
            .set_priority(priority),
        producer_task<std::chrono::milliseconds>, 4, 800ms, 3, std::ref(msgQ));

    osal::task p5(
        osal::task_attributes()
            .set_name("ProducerTask 5")
            .set_stack_size(stack_size)
            .set_priority(priority),
        producer_task<std::chrono::seconds>, 5, 2s, 3, std::ref(msgQ));

    osal::task c1(
        osal::task_attributes()
            .set_name("ConsumerTask")
            .set_stack_size(stack_size)
            .set_priority(priority),
        consumer_task, 1, 200ms, std::ref(msgQ));

    osal::this_task::sleep_for(10ms); // Block the init task so that the underlying OS can create the new tasks

    p1.join();
    p2.join();
    p3.join();
    p4.join();
    p5.join();
    c1.join();
}
