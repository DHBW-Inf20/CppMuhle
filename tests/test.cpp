#include "../source/MuhleLogik.hpp"
#include "../source/IView.hpp"
#include "../source/helperTypes.hpp"
#define BOOST_TEST_MODULE MuhleTest
#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>

class TestView: public IView
{
public:
    virtual void initialize();
    virtual void showBoard(int24 white, int24 black, bool isWhiteMove);
    virtual void showStartMenu();
    virtual void showEndScreen(std::string message);
};
void TestView::initialize(){
    return;
}
void TestView::showBoard(int24 white, int24 black, bool isWhiteMove){
    return;
}
void TestView::showStartMenu(){
    return;
}
void TestView::showEndScreen(std::string message){
    return;
}

void runLimitTests(MuhleLogik* model);


BOOST_AUTO_TEST_CASE( LogicTest )
{
    TestView tv;
    MuhleLogik testobj(&tv);
    runLimitTests(&testobj);
    testobj.initialize(true);
    BOOST_CHECK_EQUAL(0,testobj.getBlack().data);
    testobj.processInput("1");
    BOOST_CHECK_EQUAL(1,testobj.getStatus());
    BOOST_TEST_CHECKPOINT("Game Started");
    testobj.processInput("1");
    BOOST_CHECK_EQUAL(2,testobj.getWhite().data);

}

void runLimitTests(MuhleLogik* model){
    std::vector<std::string> inputs = {"kdhf","-1","","ä","5435435435"};
    for(auto input: inputs){
        std::cout << input << std::endl;
        BOOST_TEST_CHECKPOINT("Reached Input: " + input);
        model->initialize(true);
        model->processInput(input);
        
    }
    BOOST_TEST_CHECKPOINT("model.processInput() with Limits passed");

    model->initialize(true);
    model->processInput("1");
    model->processInput("1");
    BOOST_CHECK_EQUAL(0b10,model->getWhite().data);

}