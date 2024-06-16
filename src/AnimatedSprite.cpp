#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite() :
    m_animation_speed(),
    m_time(),
    m_frame_counter(0),
    m_max_frame_count(),
    m_texture(nullptr),
    m_vertex_array(sf::PrimitiveType::Triangles, 6) {}

AnimatedSprite::AnimatedSprite(sf::Texture &texture, std::size_t frame_count, sf::Time animation_speed) :
    m_animation_speed(animation_speed),
    m_time(),
    m_frame_counter(0),
    m_max_frame_count(frame_count),
    m_texture(&texture),
    m_vertex_array(sf::PrimitiveType::Triangles, 6) {}

void AnimatedSprite::update(const sf::Time& delta_time) {
    //delta_time을 더해서 경과한 시간 측정
    m_time += delta_time;

    //현재 애니메이션을 업데이트 할 필요가 없는 경우 리턴
    if (m_time < m_animation_speed) return;

    //프로그램이 랙이 걸려 정지 되었을 경우를 대비해 경과한 시간을 기준으로 얼마나 프레임이 스킵되었는지 측정
    float skipped_frame = std::floor(m_time.asSeconds() / m_animation_speed.asSeconds());

    //프레임 시간 만큼 감소 시킴
    m_time -= sf::seconds(m_animation_speed.asSeconds() * skipped_frame);
    //현재 애니메이션 프레임 구하기
    m_frame_counter += static_cast<std::size_t>(skipped_frame);

    //최대 프레임을 초과 했으면 카운터가 최대 프레임 수 보다 작아질 때 까지 최대 프레임 수 만큼 카운터 감소
    /*
     * 이유는 만약 최대 프레임 수가 3 이라고 가장하면 3번째 프레임을 출력하고 0번 째 프레임으로 되돌아가기 위해
     * 카운터를 0으로 설정하게 된다. 하지만 위와 같이 프로그램에 랙이 걸려 한번에 많은 프레임이 진행된 경우 한번에
     * 카운터가 2가 증가하여 5가 되게 된다. 그래서 이를 방지하기 위해 카운터를 최대 프레임 수 보다 작아질 만큼 빼게 되면
     * 랙 상관 없이 올바른 애니메이션 프레임을 구할 수 있다.
     */
    while (m_frame_counter >= m_max_frame_count) m_frame_counter -= m_max_frame_count;
    updateTexcoords();
}

void AnimatedSprite::setTexture(sf::Texture& texture, std::size_t frame_count) {
    //만약 텍스쳐의 주소가 같은 경우 텍스쳐를 적용하지 않음
    if (m_texture == &texture) return;

    //값 업데이트
    m_texture = &texture;
    m_max_frame_count = frame_count;

    //새로운 스프라이트가 들어왔기 때문에 에니메이션을 초기화 함
    resetAnimation();

    //정점 업데이트
    updateVertex();

    //texcoords 업데이트
    updateTexcoords();
}

const sf::Texture* AnimatedSprite::getTexture() const {
    return m_texture;
}

std::size_t AnimatedSprite::getFrameCount() const {
    return m_max_frame_count;
}

void AnimatedSprite::setAnimationSpeed(const sf::Time& speed) {
    m_animation_speed = speed;
}

const sf::Time& AnimatedSprite::getAnimationSpeed() const {
    return m_animation_speed;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const {
    return m_sprite_rect;
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const {
    return getTransform().transformRect(getLocalBounds());
}

void AnimatedSprite::resetAnimation() {
    //에니메이션을 시작 프레임으로 돌려 두어야 함으로 시간을 0으로 설정하고 카운터를 0으로 설정한다
    m_time = sf::Time::Zero;
    m_frame_counter = 0;
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    //텍스쳐가 존재하지 않으면 리턴
    if (m_texture == nullptr) return;

    //위치 변환 적용
    states.transform *= getTransform();
    //텍스쳐 지정
    states.texture = m_texture;

    //정점 배열 그리기
    target.draw(m_vertex_array, states);
}

void AnimatedSprite::updateVertex() {
    //텍스쳐 이미지 가져오기
    auto [width, height] = static_cast<sf::Vector2f>(m_texture->getSize());

    //스프라이트 범위 구하기, 값 적용
    width /= static_cast<float>(m_max_frame_count);
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
}

void AnimatedSprite::updateTexcoords() {
    //현재 프래임(지금 보여야 할 이미지)를 기반으로 texcoords의 위치를 구함
    float move = m_sprite_rect.getSize().x * static_cast<float>(m_frame_counter);
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
    m_vertex_array[0].texCoords = sf::Vector2f(move, 0.0f);
    m_vertex_array[1].texCoords = sf::Vector2f(m_sprite_rect.width + move, 0.0f);
    m_vertex_array[2].texCoords = sf::Vector2f(move, m_sprite_rect.height);

    m_vertex_array[3].texCoords = sf::Vector2f(m_sprite_rect.width + move, 0.0f);
    m_vertex_array[4].texCoords = sf::Vector2f(m_sprite_rect.width + move, m_sprite_rect.height);
    m_vertex_array[5].texCoords = sf::Vector2f(move, m_sprite_rect.height);

}


