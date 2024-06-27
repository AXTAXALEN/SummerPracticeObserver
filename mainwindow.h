// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <numeric>
#include <algorithm>

// Интерфейс Наблюдателя
class Observer {
public:
    virtual void update(const QList<int> &values) = 0;
};

// Субъект
class Subject {
    QList<Observer*> observers;

public:
    void attach(Observer* observer) {
        observers.append(observer);
    }
    void detach(Observer* observer) {
        observers.removeAll(observer);
    }
    void notify(const QList<int> &values) {
        for(auto observer : observers) {
            observer->update(values);
        }
    }
    const QList<Observer*>& getObservers() const {
        return observers;
    }
};

class LabelObserver : public QLabel, public Observer {
    Q_OBJECT

public:
    LabelObserver(QWidget *parent = nullptr) : QLabel(parent) {}

    void update(const QList<int> &values) override {
        setText(values.isEmpty() ? "" : QString("Текущее значение: %1").arg(values.last()));
    }
};

class AverageObserver : public QLabel, public Observer {
    Q_OBJECT

public:
    AverageObserver(QWidget *parent = nullptr) : QLabel(parent) {}

    void update(const QList<int> &values) override {
        if (values.isEmpty()) {
            setText("Среднее: Н/Д");
            return;
        }
        double average = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
        setText(QString("Среднее: %1").arg(average));
    }
};

class MaxObserver : public QLabel, public Observer {
    Q_OBJECT

public:
    MaxObserver(QWidget *parent = nullptr) : QLabel(parent) {}

    void update(const QList<int> &values) override {
        if (values.isEmpty()) {
            setText("Максимум: Н/Д");
            return;
        }
        int maxValue = *std::max_element(values.begin(), values.end());
        setText(QString("Максимум: %1").arg(maxValue));
    }
};

class MinObserver : public QLabel, public Observer {
    Q_OBJECT

public:
    MinObserver(QWidget *parent = nullptr) : QLabel(parent) {}

    void update(const QList<int> &values) override {
        if (values.isEmpty()) {
            setText("Минимум: Н/Д");
            return;
        }
        int minValue = *std::min_element(values.begin(), values.end());
        setText(QString("Минимум: %1").arg(minValue));
    }
};

class ArithmeticProgressionObserver : public QLabel, public Observer {
    Q_OBJECT

public:
    ArithmeticProgressionObserver(QWidget *parent = nullptr) : QLabel(parent) {}

    void update(const QList<int> &values) override {
        if (values.size() < 2) {
            setText("Не хватает данных для определения.");
            return;
        }

        // Проверка на арифметическую прогрессию
        int d = values[1] - values[0];
        bool isAP = true;

        for (int i = 2; i < values.size(); ++i) {
            if (values[i] - values[i - 1] != d) {
                isAP = false;
                break;
            }
        }

        setText(isAP ? "Последовательность является арифметической прогрессией." : "Последовательность не является арифметической прогрессией.");
    }
};

class MainWindow : public QMainWindow, public Subject {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QLineEdit *lineEdit;
    QPushButton *generateButton;
    QPushButton *themeButton;
    QPushButton *clearButton;
    QPushButton *exitButton;
    LabelObserver *labelObserver; // Объявление наблюдателей
    AverageObserver *avgObserver;
    MaxObserver *maxObserver;
    MinObserver *minObserver;
    ArithmeticProgressionObserver *apObserver; // Новый наблюдатель

    QList<int> parseInput(const QString &input);
    void setDarkThemeStyles();
    void setLightThemeStyles();

private slots:
    void generateRandomNumber();
    void toggleTheme();
    void clearLineEdit();
    void exitApplication();
};

#endif // MAINWINDOW_H
