#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(sf::Texture &texture, sf::Vector2u frame_count, sf::Time animation_speed) :
        m_animation_speed(animation_speed),
        m_vertex_need_update(true),
        m_texcoords_need_update(true),
        m_animation_time(),
        m_frame_counter(0),
        m_frame_count(frame_count),
        m_texture(&texture),
        m_vertex_array(sf::PrimitiveType::Triangles, 6) {}

void AnimatedSprite::update(const sf::Time& delta_time) {
    m_animation_time += delta_time;
    if (m_animation_time < m_animation_speed) return;

    m_animation_time -= m_animation_speed;

    m_frame_counter++;
    if (m_frame_counter >= m_frame_count.x) m_frame_counter = 0;

    m_texcoords_need_update = true;
}

bool AnimatedSprite::setTexture(sf::Texture& texture, sf::Vector2u frame_count) {
    //만약 텍스쳐의 주소가 같거나 nullptr를 받은 경우 텍스쳐를 적용하지 않음
    if (m_texture == &texture || m_texture == nullptr) return false;
    //정점 업데이트 트리거 활성화 (새 텍스쳐의 크기에 맞게 정점을 바꿔야 하기 때문)
    m_vertex_need_update = true;

    //값 업데이트
    m_texture = &texture;
    m_frame_count = frame_count;

    //새로운 스프라이트가 들어왔기 때문에 에니메이션을 초기화 함
    resetAnimation();

    return true;
}

const sf::Texture* AnimatedSprite::getTexture() const noexcept {
    return m_texture;
}

const sf::Vector2u& AnimatedSprite::getFrameCount() const noexcept {
    return m_frame_count;
}

void AnimatedSprite::setAnimationSpeed(const sf::Time& speed) {
    m_animation_speed = speed;
}

const sf::Time& AnimatedSprite::getAnimationSpeed() const noexcept {
    return m_animation_speed;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const {
    //정점 업데이트가 필요한 경우 업데이트
    if (m_vertex_need_update) updateVertex();

    return m_sprite_rect;
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const {
    //정점 업데이트가 필요한 경우 업데이트
    if (m_vertex_need_update) updateVertex();

    return getTransform().transformRect(m_sprite_rect);
}

void AnimatedSprite::resetAnimation() {
    //에니메이션을 시작 프레임으로 돌려 두어야 함으로 시간을 0으로 설정하고 카운터를 0으로 설정한다
    m_animation_time = sf::Time::Zero;
    m_frame_counter = 0;
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_vertex_need_update) updateVertex();
    if (m_texcoords_need_update) updateTexcoords();

    states.transform *= getTransform();
    states.texture = m_texture;

    target.draw(m_vertex_array, states);
}

void AnimatedSprite::updateVertex() const {
    //텍스쳐 이미지 가져오기
    sf::Vector2f texture_size = static_cast<sf::Vector2f>(m_texture->getSize());

    //스프라이트 범위 구하기, 값 적용
    float width = texture_size.x / static_cast<float>(m_frame_count.x);
    float height = texture_size.y / static_cast<float>(m_frame_count.y);
    m_sprite_rect = sf::FloatRect({ 0.0f, 0.0f }, {width, height});

    /**
     *  스프라이트 범위에 맞춰 정점 업데이트
     *  모든 기본 그래픽 요소는 삼각형으로 이루어져 있기 때문에 삼각형 2개를 사용해 사각형을 표현함
     *
     *  sf::VertexArray 클래스 사용방법이 이해가 되지 않으면 아래 튜토리얼 링크 참조
     *  https://www.sfml-dev.org/tutorials/2.6/graphics-vertex-array.php
     *
     *   정점 모양 :
     *   [0] *  *  * [1],[3]
     *   *          *     *
     *   *       *        *
     *   *    *           *
     *   [2],[5] *  *  * [4]
     *
     */
    m_vertex_array[0].position = sf::Vector2f(0.0f, 0.0f);
    m_vertex_array[1].position = sf::Vector2f(m_sprite_rect.width, 0.0f);
    m_vertex_array[2].position = sf::Vector2f(0.0f, m_sprite_rect.height);

    m_vertex_array[3].position = sf::Vector2f(m_sprite_rect.width, 0.0f);
    m_vertex_array[4].position = sf::Vector2f(m_sprite_rect.width, m_sprite_rect.height);
    m_vertex_array[5].position = sf::Vector2f(0.0f, m_sprite_rect.height);

    m_vertex_need_update = false;
}

void AnimatedSprite::updateTexcoords() const {
    //현재 프래임(지금 보여야 할 이미지)를 기반으로 texcoords의 위치를 구함
    sf::Vector2f move = m_sprite_rect.getPosition() * static_cast<float>(m_frame_counter);

    /**
     *
     *  길쭉하게 이어진 에니메이션 텍스쳐를 우리가 원하는 부분 만 보이게 자르기 위해 texcoords 값 업데이트
     *
     *  sf::VertexArray 클래스 사용방법이 이해가 되지 않으면 아래 튜토리얼 링크 참조
     *  https://www.sfml-dev.org/tutorials/2.6/graphics-vertex-array.php
     *
     *  시각화 :
     *  --------------------------------------
     *  -                      [0]  *  *  [1],[3]
     *  -                       *        *  *-
     *  -                       *   *       *-
     *  -                    [2],[5] *  *  * [4]
     *  --------------------------------------
     *
     */
    m_vertex_array[0].texCoords = sf::Vector2f(0.0f, 0.0f) + move;
    m_vertex_array[1].texCoords = sf::Vector2f(m_sprite_rect.width, 0.0f) + move;
    m_vertex_array[2].texCoords = sf::Vector2f(0.0f, m_sprite_rect.height) + move;

    m_vertex_array[3].texCoords = sf::Vector2f(m_sprite_rect.width, 0.0f) + move;
    m_vertex_array[4].texCoords = sf::Vector2f(m_sprite_rect.width, m_sprite_rect.height) + move;
    m_vertex_array[5].texCoords = sf::Vector2f(0.0f, m_sprite_rect.height) + move;

    m_texcoords_need_update = false;
}
