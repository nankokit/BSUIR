#pragma once

// Устанавливает бит bit регистра reg в 1
#define SET_BIT(reg, bit) ((reg) |= (bit))

// Устанавливает бит bit регистра reg в 0
#define CLEAR_BIT(reg, bit) ((reg) &= ~(bit))

// Инвертирует бит bit регистра reg
#define TOGGLE_BIT(reg, bit) ((reg) ^= (bit))

// Значение бита bit регистра reg
#define CHECK_BIT(reg, bit) (((reg) & (bit)) != 0)
