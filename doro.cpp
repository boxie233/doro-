#include "doro.h"
#include "config.h"
#include <QtCore/qglobal.h>
#include "mainscene.h"
doro::doro() {
    m_velocityX = 0; // 添加初始化
    m_velocityY = 0;
    m_doroRight.load(DORO_PATH);  // 向右图标路径
    m_doroLeft.load(DORO_PATH2);    // 向左图标路径
    m_doro.load(DORO_PATH);

    m_Y =( GAME_HEIGHT-m_doro.height()-50);
    m_X =( m_doro.width()-50);

    m_Rect.setWidth(m_doro.width());
    m_Rect.setHeight(m_doro.height());
    m_Rect.moveTo(m_X,m_Y);

    m_Rect.setWidth(m_doro.width() * 0.8f);
    m_Rect.setHeight(m_doro.height() * 0.8f);

    // 调整位置：中心对齐，底部留空
    m_Rect.moveTo(m_X + m_doro.width()*0.1f, m_Y + m_doro.height()*0.2f);

}



void doro::setPosition(int x,int y){
    m_X=x;
    m_Y=y;

    m_Rect.moveTo(m_X,m_Y);

};


void doro::shoot(){};

void doro::moveLeft() {
    m_velocityX = -MOVE_SPEED;  // 改为施加速度
    m_facingRight = false;
    m_X = qMax(0, m_X - MOVE_SPEED);
    m_Rect.moveTo(m_X, m_Y);
    m_facingRight = false; // 更新方向状态
}

void doro::moveRight() {
    m_velocityX = MOVE_SPEED;
    m_facingRight = true;
    int maxX = GAME_WIDTH - m_doroRight.width();
    m_X = qMin(maxX, m_X + MOVE_SPEED);
    m_Rect.moveTo(m_X, m_Y);
    m_facingRight = true; // 更新方向状态
}



void doro::jump() {
    if(m_availableJumps > 0) {
        float actualForce = (m_availableJumps == MAX_JUMPS)
        ? JUMP_FORCE
        : JUMP_FORCE * AIR_JUMP_MULTIPLIER;

        m_velocityY = actualForce;
        m_isGrounded = false;
        m_availableJumps--;
    }
}

void doro::updatePhysics() {


    if(!m_isDashing) {
        m_velocityX *= 0.1f; // 加入摩擦系数
    }

    m_X += m_velocityX; // 应用水平速度
    if (standingEnemy) {

        QRect feet = m_Rect.adjusted(0, m_Rect.height() -15, 0, 0);
        QRect enemyHead = standingEnemy->getRect().adjusted(0, -20, 0, 5);

        if(!feet.intersects(enemyHead)) {
            // 离开碰撞区域时清除站立状态
            standingEnemy = nullptr;
            m_isGrounded = false;
            m_velocityY = 0;  // 清除残留速度
            return;
        }

        standingOffsetX = m_X - standingEnemy->position.x();
        // 确保敌人存在
        if (!standingEnemy->isAlive()) {
            standingEnemy = nullptr;
            return;
        }

        // 动态跟随敌人移动
        m_X = standingEnemy->position.x() + standingOffsetX;
        m_Y = standingEnemy->position.y() - m_Rect.height() + 5;

        // 屏幕边界限制
        m_X = qBound(0, m_X, GAME_WIDTH - m_Rect.width());
        m_X = qBound(0, m_X, GAME_WIDTH - m_doroRight.width());

        // 更新碰撞框位置
        m_Rect.moveTo(m_X, m_Y);
        return;
    }

    // 常规物理计算
    if (!m_isStunned) {
        m_velocityY += GRAVITY * 0.9f;  // 增加空气阻力
        m_Y += m_velocityY;

        // 地面检测（包含敌人站立状态）
        int groundLevel = GAME_HEIGHT - m_doro.height() - 50;
        if (m_Y >= groundLevel) {
            m_Y = groundLevel;
            m_velocityY = 0;
            m_isGrounded = true;
            resetJumps();
        }
    }
    m_Rect.moveTo(m_X, m_Y);
}




void doro::resetJumps() {
    m_availableJumps = MAX_JUMPS; // 重置为最大跳跃次数
}


void doro::startStun(int duration) {
    m_isStunned = true;
    m_stunDuration = duration;
    m_stunTimer.start();
    m_velocityY = 0; // 清除下落速度

    QPixmap stunnedPixmap = m_facingRight ? m_doroRight.scaled(120, 120) :  m_doroLeft.scaled(120, 120);
    m_doro = stunnedPixmap;  // 需要添加临时成员变量
}
// doro.cpp 修改startDash和updateDash方法
void doro::startDash() {
    if (m_dashCooldownTimer.isValid() &&
        m_dashCooldownTimer.elapsed() < DASH_COOLDOWN) return;

    // 记录冲刺前状态（新增垂直速度记录）
    m_preDashSpeedX = m_velocityX;
    m_preDashSpeedY = m_velocityY; // 新增行
    m_wasGrounded = m_isGrounded;

    m_isDashing = true;
    m_dashProgress = 0;

    // 根据输入方向确定冲刺方向
    if (qAbs(m_velocityX) > 0.1f) {
        m_dashDirection = (m_velocityX > 0) ? 1 : -1;
    } else {
        m_dashDirection = m_facingRight ? 1 : -1;
    }

    // 清除垂直速度（保持直线冲刺）
    m_velocityY = 0;
    m_dashCooldownTimer.start();
}

void doro::updateDash() {
    if (!m_isDashing) return;

    // 更新残影（保持原有逻辑）
    if (m_dashProgress / DASH_DISTANCE < 0.8f) {
        addAfterimage();
    }

    // 计算移动
    float dashStep = DASH_SPEED * m_dashDirection;
    m_X += dashStep;
    m_dashProgress += qAbs(dashStep);

    // 碰撞检测（保持原有逻辑）
    checkDashCollision();

    // 结束条件
    if (m_dashProgress >= DASH_DISTANCE) {
        m_isDashing = false;

        // 恢复状态（新增垂直速度恢复）
        m_velocityX = m_preDashSpeedX;    // 恢复原水平速度
        m_velocityY = m_preDashSpeedY;    // 新增：恢复原垂直速度
        m_isGrounded = m_wasGrounded;     // 恢复地面状态

        // 如果冲刺前在空中，恢复重力影响
        if (!m_wasGrounded) {
            m_velocityY += GRAVITY * 0.5f; // 追加半帧重力补偿
        }

        // 重置冲刺相关状态
        m_dashAfterimages.clear();
        m_dashProgress = 0;
    }

    // 更新碰撞框
    m_Rect.moveTo(m_X, m_Y);
}
void doro::checkDashCollision() {
    // 碰撞检测逻辑
    QRect dashHitBox = m_Rect.adjusted(
        m_dashDirection * 20,10, m_dashDirection * 50,-10);


}

void doro::addAfterimage() {
    QPixmap current = m_facingRight ? m_doroRight : m_doroLeft;
    QPixmap afterimage = current.scaled(current.width()*1.2,current.height()*0.8);
    QPainter p(&afterimage);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(afterimage.rect(), DASH_COLOR);

    m_dashAfterimages.append(afterimage);

    // 限制残影数量
    if(m_dashAfterimages.size() > 5) {
        m_dashAfterimages.removeFirst();
    }
}

