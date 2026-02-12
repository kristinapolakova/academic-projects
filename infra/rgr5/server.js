const express = require('express');
const app = express();
const PORT = 3000;

// Middleware для парсинга JSON в теле POST запросов
app.use(express.json());

const quotes = [
    "Живи здесь и сейчас.",
    "Каждый день — новый шанс.",
    "Ошибки — часть обучения."
];

const advices = [
    "Пей больше воды.",
    "Регулярно отдыхай.",
    "Занимайся спортом",
    "hi"
];

let messages = [];

app.get('/random-number', (req, res) => {
    const number = Math.floor(Math.random() * 100) + 1;
    res.json({ number });
});

app.get('/quote', (req, res) => {
    const quote = quotes[Math.floor(Math.random() * quotes.length)];
    res.json({ quote });
});

app.get('/advice', (req, res) => {
    const advice = advices[Math.floor(Math.random() * advices.length)];
    res.json({ advice });
});

// Получаем все сообщения, сохранённые через POST
app.get('/messages', (req, res) => {
    res.json(messages);
});

// POST запрос для отправки сообщения
app.post('/message', (req, res) => {
    const { user, text } = req.body;
    if (!user || !text) {
        return res.status(400).json({ error: 'user и text обязательны' });
    }
    const message = { user, text, timestamp: new Date() };
    messages.push(message);
    res.status(201).json({ success: true, message });
});

function shutdown() {
    console.log('\nПолучен сигнал остановки, завершаем работу...');
    process.exit(0);
}

process.on('SIGTERM', shutdown);
process.on('SIGINT', shutdown);

app.listen(PORT, () => {
    console.log(`Server listening on port ${PORT}`);
});
