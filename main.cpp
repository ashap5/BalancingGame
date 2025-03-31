#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>

class BalancingGame : public QWidget {
    int barLength, barWeight, playerLength, playerWeight;
    int lengths[4] = {1, 2, 4, 8};
    int weights[5] = {8, 16, 24, 32, 40};
    QLineEdit *input;
    QLabel *instructionLabel;

public:
    BalancingGame(QWidget *parent = nullptr) : QWidget(parent) {
        srand(time(0));
        resetGame();
        setFixedSize(1000, 680);

        // Replace QVBoxLayout with QHBoxLayout for the input area
        QWidget *inputWidget = new QWidget(this);
        QHBoxLayout *inputLayout = new QHBoxLayout(inputWidget);
        inputLayout->setContentsMargins(10, 10, 10, 0);  // Add margins to position at the top left
        
        instructionLabel = new QLabel("Enter the correct weight to balance the bar:", this);
        inputLayout->addWidget(instructionLabel);

        input = new QLineEdit(this);
        input->setFixedWidth(100);
        inputLayout->addWidget(input);

        QPushButton *submitButton = new QPushButton("Submit", this);
        inputLayout->addWidget(submitButton);
        
        // Add stretch to push everything to the left
        inputLayout->addStretch();
        
        // Position the input widget at the top of the window
        inputWidget->setGeometry(0, 0, width(), 50);
        
        connect(submitButton, &QPushButton::clicked, this, &BalancingGame::checkBalance);
        // Connect Enter key to also check balance
        connect(input, &QLineEdit::returnPressed, this, &BalancingGame::checkBalance);
    }

    void resetGame() {
        barLength = lengths[rand() % 4];
        barWeight = weights[rand() % 5];
        playerLength = lengths[rand() % 4];
        playerWeight = 0;
        // if (input) {  // Add null check before clearing
        //     input->clear();
        // }
        update();
    }

    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int unit = 40;
        int pivotX = width() / 2;
        int pivotY = height() / 2;

        // Draw bar
        painter.setPen(Qt::white);
        painter.drawLine(pivotX - (barLength * unit), pivotY, pivotX + (playerLength * unit), pivotY);

        // Mark pivot point
        painter.setBrush(Qt::white);
        painter.drawEllipse(pivotX - 5, pivotY - 5, 10, 10);

        // Draw left weight
        painter.setBrush(Qt::red);
        painter.drawEllipse(pivotX - (barLength * unit) - 15, pivotY - 15, 30, 30);
        painter.drawText(pivotX - (barLength * unit) - 10, pivotY - 20, QString::number(barWeight));
        
        // Display the left bar length to the left of the ball (moved further left)
        painter.setPen(Qt::white);
        painter.drawText(pivotX - (barLength * unit) - 90, pivotY + 5, QString("Length: %1").arg(barLength));

        // Draw right weight (user selection)
        painter.setBrush(Qt::blue);
        painter.drawEllipse(pivotX + (playerLength * unit) - 15, pivotY - 15, 30, 30);
        painter.drawText(pivotX + (playerLength * unit) - 10, pivotY - 20, playerWeight == 0 ? "?" : QString::number(playerWeight));
        
        // Display the right bar length at the outer side of the right ball
        painter.setPen(Qt::white);
        painter.drawText(pivotX + (playerLength * unit) + 20, pivotY + 5, QString("Length: %1").arg(playerLength));
    }

    void checkBalance() {
        bool ok;
        int userWeight = input->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid number.");
            return;
        }
        playerWeight = userWeight;
        update();

        if (barLength * barWeight == playerLength * playerWeight) {
            QMessageBox::information(this, "Result", "Correct! The bar is balanced.");
        } else {
            QMessageBox::information(this, "Result", "Incorrect! The correct weight was " + QString::number((barLength * barWeight) / playerLength));
        }
        resetGame();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    BalancingGame window;
    window.setWindowTitle("Balancing Game");
    window.show();
    return app.exec();
}
