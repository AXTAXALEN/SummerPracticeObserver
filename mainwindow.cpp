// mainwindow.cpp
#include "mainwindow.h"
#include <random>
#include <QRegularExpression>
#include <QStringList>
#include <chrono>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QKeyEvent>
#include <QRegularExpressionValidator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    lineEdit = new QLineEdit(this);
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("-?[0-9, ]+"), this);
    lineEdit->setValidator(validator);

    generateButton = new QPushButton("Сгенерировать число", this);
    themeButton = new QPushButton("Сменить тему", this);
    clearButton = new QPushButton("Очистить строку", this);
    exitButton = new QPushButton("Выйти", this);

    // Настройка стилей для кнопок
    QString buttonStyle = "QPushButton { background-color: #4CAF50; color: white; border: 2px solid #4CAF50; padding: 8px; border-radius: 5px; }";
    generateButton->setStyleSheet(buttonStyle);
    themeButton->setStyleSheet(buttonStyle);
    clearButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    generateButton->setCursor(Qt::PointingHandCursor);
    themeButton->setCursor(Qt::PointingHandCursor);
    clearButton->setCursor(Qt::PointingHandCursor);
    exitButton->setCursor(Qt::PointingHandCursor);

    connect(themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearLineEdit);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitApplication);

    labelObserver = new LabelObserver(this);
    avgObserver = new AverageObserver(this);
    maxObserver = new MaxObserver(this);
    minObserver = new MinObserver(this);
    apObserver = new ArithmeticProgressionObserver(this); // Новый наблюдатель

    attach(labelObserver);
    attach(avgObserver);
    attach(maxObserver);
    attach(minObserver);
    attach(apObserver); // Регистрация нового наблюдателя

    connect(lineEdit, &QLineEdit::textChanged, [this](const QString &text) {
        QList<int> values = parseInput(text);
        notify(values);
    });

    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateRandomNumber);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(generateButton);
    buttonLayout->addWidget(themeButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(exitButton);
    buttonLayout->addStretch();

    layout->addWidget(lineEdit);
    layout->addLayout(buttonLayout);
    layout->addWidget(labelObserver);
    layout->addWidget(avgObserver);
    layout->addWidget(maxObserver);
    layout->addWidget(minObserver);
    layout->addWidget(apObserver); // Добавление нового наблюдателя в компоновку

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Тёмная тема
    setDarkThemeStyles();

    // Полноэкранный режим
    showFullScreen();
}

MainWindow::~MainWindow() {}

QList<int> MainWindow::parseInput(const QString &input) {
    QList<int> values;
    QRegularExpression re("\\s*,\\s*");
    QStringList stringList = input.split(re, QString::SkipEmptyParts);
    for (const QString &str : stringList) {
        bool ok;
        int value = str.toInt(&ok);
        if (ok) {
            values.append(value);
        }
    }
    return values;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}

void MainWindow::generateRandomNumber() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned long>(seed));
    std::uniform_int_distribution<> dis(-100, 100);
    int randomValue = dis(gen);
    QString currentText = lineEdit->text().trimmed();
    if (!currentText.isEmpty()) {
        currentText.append(", ");
    }
    currentText.append(QString::number(randomValue));
    lineEdit->setText(currentText);
}

void MainWindow::toggleTheme() {
    static bool darkTheme = false;
    darkTheme = !darkTheme;

    if (darkTheme) {
        setDarkThemeStyles();
    } else {
        setLightThemeStyles();
    }
}

void MainWindow::clearLineEdit() {
    lineEdit->clear();
}

void MainWindow::exitApplication() {
    close();
}

void MainWindow::setDarkThemeStyles() {
    QString darkBackgroundColor = "#212121";
    QString darkTextColor = "#ffffff";
    QString darkLineEditBackgroundColor = "#383838";

    setStyleSheet("QMainWindow { background-color: " + darkBackgroundColor + "; color: " + darkTextColor + "; }");
    centralWidget()->setStyleSheet("background-color: " + darkBackgroundColor + "; color: " + darkTextColor + ";");
    lineEdit->setStyleSheet("QLineEdit { background-color: " + darkLineEditBackgroundColor + "; border: 2px solid #4CAF50; padding: 8px; border-radius: 5px; color: " + darkTextColor + "; }");

    QString buttonStyle = "QPushButton { background-color: #757575; color: white; border: 2px solid #757575; padding: 8px; border-radius: 5px; }";
    generateButton->setStyleSheet(buttonStyle);
    themeButton->setStyleSheet(buttonStyle);
    clearButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    labelObserver->setStyleSheet("QLabel { background-color: #424242; padding: 8px; border: 2px solid #757575; border-radius: 5px; color: " + darkTextColor + "; }");
    avgObserver->setStyleSheet("QLabel { background-color: #1A237E; padding: 8px; border: 2px solid #3949AB; border-radius: 5px; color: " + darkTextColor + "; }");
    maxObserver->setStyleSheet("QLabel { background-color: rgba(30, 130, 76, 80); padding: 8px; border: 2px solid #388E3C; border-radius: 5px; color: " + darkTextColor + "; }");
    minObserver->setStyleSheet("QLabel { background-color: rgba(183, 28, 28, 80); padding: 8px; border: 2px solid #D32F2F; border-radius: 5px; color: " + darkTextColor + "; }");
    apObserver->setStyleSheet("QLabel { background-color: #212121; padding: 8px; border: 2px solid #4CAF50; border-radius: 5px; color: " + darkTextColor + "; }"); // Стили нового наблюдателя
}

void MainWindow::setLightThemeStyles() {
    QString lightBackgroundColor = "#f0f0f0";
    QString lightTextColor = "#000000";

    setStyleSheet("QMainWindow { background-color: " + lightBackgroundColor + "; color: " + lightTextColor + "; }");
    centralWidget()->setStyleSheet("background-color: " + lightBackgroundColor + "; color: " + lightTextColor + ";");
    lineEdit->setStyleSheet("QLineEdit { background-color: #f0f0f0; border: 2px solid #4CAF50; padding: 8px; border-radius: 5px; color: " + lightTextColor + "; }");

    QString buttonStyle = "QPushButton { background-color: #4CAF50; color: white; border: 2px solid #4CAF50; padding: 8px; border-radius: 5px; }";
    generateButton->setStyleSheet(buttonStyle);
    themeButton->setStyleSheet(buttonStyle);
    clearButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    labelObserver->setStyleSheet("QLabel { background-color: #FFFFE0; padding: 8px; border: 2px solid #FFD700; border-radius: 5px; color: " + lightTextColor + "; }");
    avgObserver->setStyleSheet("QLabel { background-color: #E0F7FA; padding: 8px; border: 2px solid #B2EBF2; border-radius: 5px; color: " + lightTextColor + "; }");
    maxObserver->setStyleSheet("QLabel { background-color: #E8F5E9; padding: 8px; border: 2px solid #A5D6A7; border-radius: 5px; color: " + lightTextColor + "; }");
    minObserver->setStyleSheet("QLabel { background-color: #FFEBEE; padding: 8px; border: 2px solid #EF5350; border-radius: 5px; color: " + lightTextColor + "; }");
    apObserver->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 8px; border: 2px solid #4CAF50; border-radius: 5px; color: " + lightTextColor + "; }"); // Стили нового наблюдателя
}
