const { match } = require('assert');
const fs = require('fs');



function parseCards(input) {
    const gamesInput = input.trim().split('\n')
    const games = [];
    for (let game of gamesInput) {
        const cardNumberRe = /Card\s+(\d+):/g
        const cardNumbermatches = cardNumberRe.exec(game)
        const numbersRe = /:\s+([^\|]+)\s+\|\s+(.+)/g
        const numberMatches = numbersRe.exec(game)
        const winning = new Set(numberMatches[1].split(/\s+/))
        const owned = new Set(numberMatches[2].split(/\s+/))
        games.push({
            number: parseInt(cardNumbermatches[1]),
            winning: winning,
            owned: owned,
        })
    }
    return games
}

function puzzle1(cards) {
    let sum = 0
    for (let card of cards) {
        const intersection = new Set([...card.winning].filter(n => card.owned.has(n)))
        if (intersection.size > 0) {
            sum += Math.pow(2, intersection.size - 1)
        }
    }
    return sum
}

function puzzle2(cards) {
    const totalOwned = {}
    for (let card of cards) {
        totalOwned[card.number] = 0
    }
    for (let card of cards) {
        totalOwned[card.number]++
        const intersection = new Set([...card.winning].filter(n => card.owned.has(n)))
        for (let won = 1; won <= intersection.size; won++) {
            totalOwned[card.number + won] += totalOwned[card.number]
        }
    }
    let sum = 0
    for (let cardNumber in totalOwned) {
        sum += totalOwned[cardNumber]
    }
    return sum
}

function main(filename) {
    fs.readFile(filename, { encoding: "utf-8" }, function (err, data) {
        const cards = parseCards(data)
        const puzzle1Answer = puzzle1(cards)
        console.log("Puzzle 1 answer: ", puzzle1Answer)
        const puzzle2Answer = puzzle2(cards)
        console.log("Puzzle 2 answer: ", puzzle2Answer)

    });
}

main("./input.txt")