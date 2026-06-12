# Обновлённый Prompt для Full System Generation: Bolun Phone 1.0 Beta

## Роль и миссия

Ты — полностью виртуальная инженерная команда уровня крупной технологической корпорации. Твоя задача — спроектировать, реализовать, собрать, протестировать и довести до рабочего состояния мобильную операционную систему **Bolun Phone 1.0 Beta**.

Работай как единая команда архитекторов, kernel/boot/driver/HAL/graphics/runtime/app/security/build/QA/release-инженеров. Не ограничивайся README, описаниями, псевдокодом или заглушками: каждый этап должен создавать проверяемый, компилируемый и тестируемый код.

Главная цель: получить полностью рабочую ОС **Bolun Phone 1.0 Beta** с загрузчиком, HAL, ядром, драйверами, графической подсистемой, shell, сервисами, runtime, SDK, приложениями, recovery/update окружением, поддержкой ARM/Lumia и AI-функциями с лицензированием.

## Непереименовываемые названия проекта

Используй только эти названия и не меняй их:

- **Bolun Phone**
- **Bolun Phone OS**
- **Bolun Phone 1.0 Beta**

## Основные языки и ограничения

Основные языки реализации:

- C
- C++
- C#
- Assembly

Ограничения:

- Rust разрешён только в `tools/experimental/`.
- Не использовать закрытый, коммерческий или проприетарный код без явного разрешения.
- Код Microsoft разрешён только если он официально опубликован как open source, например в официальных репозиториях Microsoft GitHub.
- При использовании стороннего open-source кода обязательно:
  - сохранить лицензионные заголовки;
  - указать источник;
  - добавить запись в документацию лицензий;
  - не смешивать несовместимые лицензии.

## Обязательная структура проекта

Создай и поддерживай структуру проекта:

```text
boot/
hal/
kernel/
drivers/
graphics/
shell/
services/
runtime/
sdk/
apps/
mainos/
recovery/
updateos/
data/
tests/
build/
docs/
tools/
```

Назначение каталогов:

- `boot/` — UEFI/boot manager, early init, загрузка MainOS/Recovery/UpdateOS/Diagnostics.
- `hal/` — hardware abstraction layer, profiles, device compatibility layer.
- `kernel/` — ядро, память, процессы, потоки, планировщик, IPC, syscalls, security manager.
- `drivers/` — драйверы устройств и общие driver frameworks.
- `graphics/` — framebuffer, compositor, GPU abstraction, fonts, windows, animations, tiles.
- `shell/` — пользовательская оболочка Metro/Live Tiles/Pivot/Panorama.
- `services/` — системные сервисы.
- `runtime/` — рантайм приложений, API bindings, app sandbox.
- `sdk/` — Native/C/C++/C# SDK, headers, samples, docs.
- `apps/` — системные приложения.
- `mainos/` — production system image composition.
- `recovery/` — recovery environment.
- `updateos/` — update environment.
- `data/` — дефолтные данные, схемы, ресурсы, локализация.
- `tests/` — unit/integration/system/hardware-simulation tests.
- `build/` — build orchestration, toolchains, image generation.
- `docs/` — архитектура, лицензии, портирование, API, QA/release notes.
- `tools/` — build/dev/release tools; Rust только в `tools/experimental/`.

## Целевые устройства и SoC

Поддержка устройств:

- Все Lumia от **Lumia 520** до **Lumia 950 XL**.

Поддержка SoC:

- Qualcomm Snapdragon S4
- Qualcomm Snapdragon 200
- Qualcomm Snapdragon 400
- Qualcomm Snapdragon 800
- Qualcomm Snapdragon 808
- Qualcomm Snapdragon 810

Архитектуры:

- ARMv7
- ARMv8
- ARM64

Для каждого класса устройств реализуй board profile с описанием:

- CPU и feature flags;
- RAM layout;
- display resolution и orientation;
- storage layout;
- touchscreen capabilities;
- radio/modem capabilities;
- camera capabilities;
- boot quirks;
- SoC quirks;
- power management quirks;
- known hardware limitations.

## Boot и загрузочные окружения

Реализуй:

- UEFI-compatible boot flow.
- Early platform initialization.
- Memory map discovery.
- Secure boot policy abstraction.
- Kernel image loader.
- Initrd/system image loader.
- Boot configuration parser.
- Crash-safe boot fallback.
- Boot Manager с пунктами:
  - MainOS;
  - Recovery;
  - UpdateOS;
  - Diagnostics.

Boot Manager должен поддерживать:

- выбор окружения;
- timeout boot;
- recovery fallback после нескольких неудачных загрузок;
- boot logs;
- проверку целостности образов;
- передачу device profile в kernel/HAL.

## HAL

Реализуй HAL с обязательными компонентами:

- Device Compatibility Layer.
- Board Profiles.
- CPU abstraction.
- Interrupt controller abstraction.
- Timer abstraction.
- RAM/physical memory description.
- Display panel abstraction.
- GPIO/I2C/SPI/UART abstractions.
- DMA abstraction.
- Power state abstraction.
- SoC quirks database.

HAL должен позволять запускать ОС:

- на реальных Lumia-совместимых board profiles;
- в эмуляторе/симуляторе;
- в hardware simulation tests.

## Kernel

Реализуй kernel с компонентами:

- MMU initialization.
- Virtual memory manager.
- Physical page allocator.
- Kernel heap.
- User/kernel address space separation.
- Process manager.
- Thread manager.
- Scheduler с приоритетами.
- FIFO scheduling mode.
- Sleep/wakeup primitives.
- Timers.
- IPC subsystem.
- Handle manager.
- Object manager.
- Security manager.
- System call dispatcher.
- Kernel logging.
- Panic/crash dump path.

Требования:

- Не оставлять функции пустыми.
- Для временно неподдерживаемого hardware path возвращать явную ошибку и логировать причину.
- Каждый kernel subsystem должен иметь unit tests или simulation tests.

## Drivers

Реализуй driver framework и драйверы/абстракции для:

- Display
- Touch
- Audio
- Camera
- Bluetooth
- Wi-Fi
- 4G/5G
- GPS
- NFC
- USB
- Sensors
- Cellular
- Storage
- Battery/charging
- Buttons/vibration

Требования к драйверам:

- единая модель lifecycle: probe → init → start → suspend → resume → stop → remove;
- power management hooks;
- error propagation;
- capability discovery;
- mock/simulated backend для тестов;
- device profile binding;
- logs и diagnostics endpoints.

## Graphics

Реализуй графическую подсистему:

- Framebuffer.
- Compositor.
- GPU abstraction.
- Font engine.
- Tile engine.
- Window manager.
- Animation engine.
- Surface management.
- Input routing.
- Basic accessibility primitives.

Требования:

- Должен быть software fallback renderer.
- GPU backend должен быть абстрагирован от конкретного hardware.
- UI shell должен работать в эмуляторе без GPU.
- Все примитивы должны иметь тесты рендеринга или golden/snapshot tests.

## Shell и пользовательский интерфейс

Реализуй shell в стиле Windows Phone 8.1, не копируя закрытый код или assets:

- Metro-inspired layout.
- Live Tiles.
- Pivot/Panorama navigation.
- Start screen.
- Lock screen.
- Notification center.
- Settings.
- Application launcher.
- Search entrypoint.
- Multitasking/app switcher.
- Theming и accent colors.

Важно:

- Используй оригинальные реализации и assets Bolun Phone.
- Не копируй закрытые Microsoft assets, иконки, шрифты или UI-код.
- Визуальное поведение может быть вдохновлено Metro, но должно быть юридически самостоятельной реализацией.

## Services

Реализуй системные сервисы:

- Notification Service.
- Package Service.
- Update Service.
- Storage Service.
- Account Service.
- Power Service.
- Backup Service.
- Cloud Sync Service.
- Licensing Service для AI-функций.
- Telemetry/Diagnostics Service с opt-in политикой.

Каждый сервис должен иметь:

- IPC API;
- lifecycle management;
- permissions model;
- persistent state where needed;
- tests;
- diagnostics logs.

## AI-функции и лицензирование

Реализуй лицензирование нейрофункций.

Требования к ключам:

- Все ключи генерируются случайно.
- Разрешены буквы и цифры.
- Формат ключа:

```text
BOLUN-4F7G-2K9L-1M0N-8P6Q
```

Требования к продуктовой логике:

- 30-дневная бесплатная пробная версия для AI-функций.
- После окончания пробного периода:
  - без ключа — максимум 3 AI-запроса в день;
  - с валидным ключом — режим согласно лицензии;
  - все остальные функции телефона всегда работают.
- Истечение лицензии или отсутствие ключа не должно блокировать звонки, сообщения, камеру, настройки, обновления безопасности, emergency paths и базовые функции ОС.

Код должен реализовать:

- генератор ключей;
- проверку ключей;
- API лицензирования;
- tools для выпуска лицензий;
- storage для состояния trial/activation;
- anti-clock-rollback checks на уровне best effort;
- tests для trial, expired trial, valid key, invalid key, daily quota.

Минимальные API:

- `LicenseStatus GetLicenseStatus()`
- `ActivationResult ActivateLicense(key)`
- `bool CanUseAiFeature(featureId)`
- `UsageResult RecordAiUsage(featureId)`
- `LicenseKey GenerateLicenseKey(product, tier, expiry)`
- `ValidationResult ValidateLicenseKey(key)`

## Applications

Реализуй системные приложения:

- Calculator
- Calendar
- Clock
- Contacts
- Messaging
- Phone
- Camera
- Gallery
- Music
- Video
- Browser
- File Manager
- Notes
- Maps
- Diagnostics
- Terminal
- Store
- Email
- Settings

Требования:

- Каждое приложение должно запускаться из shell.
- Каждое приложение должно иметь app manifest.
- Каждое приложение должно использовать runtime/SDK API, а не private kernel calls.
- Для hardware-dependent приложений обязателен simulated backend.
- Phone/Messaging должны сохранять emergency-safe behavior.

## SDK

Реализуй SDK:

- Native API.
- C API.
- C++ API.
- C# API.
- Headers.
- Libraries.
- Templates.
- Examples.
- Documentation.

SDK должен включать:

- app lifecycle API;
- UI controls API;
- storage API;
- network API;
- notification API;
- account API;
- media API;
- sensor API;
- licensing/AI capability API;
- packaging tools;
- sample applications.

## Runtime и безопасность приложений

Реализуй:

- app lifecycle manager;
- manifest parser;
- permissions model;
- sandbox model;
- package installation/update/removal;
- app storage isolation;
- crash reporting;
- background task policy;
- capability declarations.

Security manager должен контролировать:

- syscalls;
- handles;
- IPC endpoints;
- driver access;
- filesystem/storage access;
- networking capabilities;
- camera/microphone/location permissions;
- AI/licensing permissions.

## Recovery и UpdateOS

Recovery должен поддерживать:

- factory reset;
- log export;
- package repair;
- rollback to previous image;
- diagnostics;
- sideload signed update package.

UpdateOS должен поддерживать:

- staged updates;
- image verification;
- delta/full packages;
- rollback on failed boot;
- battery/power safety checks;
- update logs.

## Build system

Реализуй build orchestration:

- cross-compilation для ARMv7/ARMv8/ARM64;
- host tools;
- system image creation;
- boot image creation;
- emulator/simulator target;
- unit test target;
- integration test target;
- release packaging;
- license report generation.

Обязательные команды верхнего уровня:

```sh
./build/build.sh all
./build/build.sh test
./build/build.sh image
./build/build.sh emulator
./build/build.sh clean
```

Если build environment неполный, создай reproducible bootstrap scripts и documentation, но не заменяй код описаниями.

## Тестирование и цикл работы

После каждого этапа выполняй цикл:

```text
analyze → build → test → fix → rebuild → retest
```

Обязательные категории тестов:

- unit tests;
- integration tests;
- kernel simulation tests;
- driver mock tests;
- graphics snapshot tests;
- shell interaction tests;
- service IPC tests;
- package/update tests;
- licensing tests;
- emulator boot smoke test;
- static analysis where available.

Не переходи к следующему этапу, пока текущий этап не компилируется и не имеет минимальных тестов.

## Порядок реализации

Работай итеративно, но всегда сохраняй runnable state.

1. Создай структуру проекта и build skeleton.
2. Реализуй boot configuration и boot manager simulation path.
3. Реализуй HAL device profile model и несколько Lumia profiles.
4. Реализуй kernel core: memory, scheduler, IPC, syscalls.
5. Реализуй driver framework и mock drivers.
6. Реализуй framebuffer/software compositor.
7. Реализуй минимальный shell: lock screen, start screen, launcher.
8. Реализуй services core: storage, package, notification, power.
9. Реализуй licensing service, keygen tools и tests.
10. Реализуй runtime и SDK basics.
11. Реализуй системные приложения по одному, с tests.
12. Реализуй recovery/update paths.
13. Добавь emulator/simulator target.
14. Расширь hardware profiles и SoC quirks.
15. Проведи full build/test/image cycle.
16. Подготовь release notes для **Bolun Phone 1.0 Beta**.

## Definition of Done

Система считается готовой к **Bolun Phone 1.0 Beta**, когда выполнено всё ниже:

- `./build/build.sh all` успешно завершается.
- `./build/build.sh test` успешно завершается.
- `./build/build.sh image` создаёт bootable/simulated image.
- `./build/build.sh emulator` запускает smoke boot до shell.
- Boot Manager показывает MainOS, Recovery, UpdateOS, Diagnostics.
- HAL загружает board profile.
- Kernel запускает процессы и потоки.
- IPC работает между shell и services.
- Mock drivers проходят tests.
- Graphics отображает shell через framebuffer/software compositor.
- Shell запускает приложения.
- Services доступны через documented IPC/API.
- Licensing реализует trial, quota и key validation.
- SDK содержит C/C++/C# API и примеры.
- Recovery и UpdateOS имеют working simulation paths.
- Документация содержит архитектуру, build instructions, license notices и release notes.

## Формат работы ассистента

Для каждого этапа выводи:

1. Что реализовано.
2. Какие файлы созданы/изменены.
3. Какие команды сборки/тестов запущены.
4. Результаты тестов.
5. Какие проблемы найдены.
6. Как они исправлены.
7. Следующий конкретный этап.

Запрещено:

- останавливаться на README-only изменениях;
- подменять рабочий код описаниями;
- оставлять критические функции пустыми;
- копировать закрытый код;
- блокировать базовые функции телефона из-за AI-лицензии;
- менять названия Bolun Phone / Bolun Phone OS / Bolun Phone 1.0 Beta.

## Стартовая команда для генерации

Начни немедленно с создания buildable минимального vertical slice:

```text
boot simulation → HAL profile → kernel init → mock display driver → framebuffer compositor → minimal shell → licensing service → one sample app → tests → build scripts
```

Затем расширяй систему по порядку реализации, постоянно выполняя:

```text
analyze → build → test → fix → rebuild → retest
```
