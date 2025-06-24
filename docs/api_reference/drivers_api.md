# Synapse Drivers Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის დრაივერების (drivers) მოდულების სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## I2C Bus Driver API

### module_t *i2c_bus_module_create(const cJSON *config);
- ქმნის ახალი I2C Bus მოდულის ინსტანციას გადაცემული კონფიგურაციით.

### esp_err_t i2c_bus_init(i2c_bus_handle_t *i2c_bus_handle, const i2c_bus_config_t *config);
- ინიციალიზაციას უკეთებს I2C ავტობუსს.

### esp_err_t i2c_bus_read(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, uint8_t *data, size_t data_length);
- კითხულობს მონაცემებს I2C მოწყობილობიდან.

### esp_err_t i2c_bus_write(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, const uint8_t *data, size_t data_length);
- წერს მონაცემებს I2C მოწყობილობაში.

### esp_err_t i2c_bus_scan(i2c_bus_handle_t *i2c_bus_handle);
- ასკანერებს ავტობუსზე დაკავშირებულ მოწყობილობებს.

---

## SPI Bus Driver API

### module_t *spi_bus_module_create(const cJSON *config);
- ქმნის ახალი SPI Bus მოდულის ინსტანციას გადაცემული კონფიგურაციით.

### esp_err_t spi_bus_init(spi_bus_handle_t *spi_bus_handle, const spi_bus_config_t *config);
- ინიციალიზაციას უკეთებს SPI ავტობუსს.

### esp_err_t spi_bus_transfer(spi_bus_handle_t *spi_bus_handle, const uint8_t *tx_data, uint8_t *rx_data, size_t data_length);
- ასრულებს SPI გადაცემას.

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**I2C Read:**
```c
i2c_bus_read(i2c_bus_handle, 0x3C, data, data_length);
```

**SPI Transfer:**
```c
spi_bus_transfer(spi_bus_handle, tx_data, rx_data, data_length);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

